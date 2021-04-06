//
// Created by kosmas on 28/1/21.
//

#include "system_PSP.h"
#include <pspmp3.h>
#include <pspaudio.h>
#define MAININCLUDE 1
#include "player_PSP_pspaudio.h"
#undef MAININCLUDE

int fillStreamBuffer( int fd, int handle ) {
    SceUChar8 *dst;
    int write;
    int pos;
    // Get Info on the stream (where to fill to, how much to fill, where to fill from)
    status = sceMp3GetInfoToAddStreamData(handle, &dst, &write, &pos);
    if (status < 0) {
        ERRORMSG("ERROR: sceMp3GetInfoToAddStreamData returned 0x%08X\n", status);
    }

    // Seek file to requested position
    status = sceIoLseek32(fd, pos, SEEK_SET);
    if (status < 0) {
        ERRORMSG("ERROR: sceIoLseek32 returned 0x%08X\n", status);
    }

    // Read data
    int read = sceIoRead(fd, dst, write);
    if (read < 0) {
        ERRORMSG("ERROR: Could not read from file - 0x%08X\n", read);
    }

    if (read == 0) {
        // End of file?
        return 0;
    }

    // Notify mp3 library about how much we really wrote to the stream buffer
    status = sceMp3NotifyAddStreamData(handle, read);
    if (status < 0) {
        ERRORMSG("ERROR: sceMp3NotifyAddStreamData returned 0x%08X\n", status);
    }

    return (pos>0);
}

void LoadFile() {
    fd = sceIoOpen(pathToPlay.c_str(), PSP_O_RDONLY, 0777);
    if (fd<0)
    {
        ERRORMSG("ERROR: Could not open file '%s' - 0x%08X\n", pathToPlay, fd);
    }

    // Init mp3 resources
    status = sceMp3InitResource();
    if (status<0)
    {
        ERRORMSG("ERROR: sceMp3InitResource returned 0x%08X\n", status);
    }

    // Reserve a mp3 handle for our playback
    SceMp3InitArg mp3Init;
    mp3Init.mp3StreamStart = 0;
    mp3Init.mp3StreamEnd = sceIoLseek32(fd, 0, SEEK_END);
    mp3Init.unk1 = 0;
    mp3Init.unk2 = 0;
    mp3Init.mp3Buf = mp3Buf;
    mp3Init.mp3BufSize = sizeof(mp3Buf);
    mp3Init.pcmBuf = pcmBuf;
    mp3Init.pcmBufSize = sizeof(pcmBuf);

    handle = sceMp3ReserveMp3Handle(&mp3Init);
    if (handle < 0) {
        ERRORMSG("ERROR: sceMp3ReserveMp3Handle returned 0x%08X\n", handle);
    }

    // Fill the stream buffer with some data so that sceMp3Init has something to work with
    fillStreamBuffer(fd, handle);

    status = sceMp3Init(handle);
    if (status < 0) {
        ERRORMSG("ERROR: sceMp3Init returned 0x%08X\n", status);
    }

    channel = -1;
    samplingRate = sceMp3GetSamplingRate(handle);
    numChannels = sceMp3GetMp3ChannelNum(handle);
    lastDecoded = 0;
    volume = PSP_AUDIO_VOLUME_MAX;
    numPlayed = 0;
    paused = 0;
    loop = 0;
}

void PlaySource() {
    LoadFile();
    pspDebugScreenPrintf("Now playing: %s", pathToPlay.c_str());
    while (true) {
        if(ProcessInput()) {
            break;
        }
        if (!paused) {
            // Fill buffer if needed
            if (sceMp3CheckStreamDataNeeded(handle) > 0) {
                fillStreamBuffer(fd, handle);
            }

            // Decode some samples
            short *buf;
            int retries = 0;
            // We retry in case it's just that we reached the end of the stream and need to loop
            for (; retries < 1; retries++) {
                bytesDecoded = sceMp3Decode(handle, &buf);
                if (bytesDecoded > 0) {
                    break;
                }

                if (sceMp3CheckStreamDataNeeded(handle) <= 0) {
                    break;
                }

                if (!fillStreamBuffer(fd, handle)) {
                    numPlayed = 0;
                }
            }
            if (bytesDecoded < 0 && bytesDecoded != 0x80671402) {
                ERRORMSG("ERROR: sceMp3Decode returned 0x%08X\n", bytesDecoded);
            }

            // If this is true we have reached the end of the input buffer
            if (bytesDecoded == 0 || bytesDecoded == 0x80671402) {
                paused = 1;
            } else {
                // Reserve audio channel if not yet done
                if (channel < 0 || lastDecoded != bytesDecoded) {
                    if (channel >= 0) {
                        sceAudioSRCChRelease();
                    }

                    channel = sceAudioSRCChReserve(bytesDecoded / (2 * numChannels), samplingRate, numChannels);
                }
                // Output the decoded samples and accumulate the number of played samples to get the playtime
                numPlayed += sceAudioSRCOutputBlocking(volume, buf);
            }
        }
    }
}

bool isPlaying() {
    return false;
}

/*
Normally called by main(). However, with pspaudio needing an infinite
loop to output audio this would need to hold the audio playing code. Instead,
PlayFile() holds an infinite loop and calls this. main() never calls this.
*/
int ProcessInput() {
    oldCtrlData = ctrlData;
    sceCtrlPeekBufferPositive(&ctrlData, 1);
    if (ctrlData.Buttons != oldCtrlData.Buttons) {
        if (ctrlData.Buttons & PSP_CTRL_CIRCLE) {
            paused ^= 1;
        }

        if (ctrlData.Buttons & PSP_CTRL_TRIANGLE) {
            // Reset the stream and playback status
            sceMp3ResetPlayPosition(handle);
            numPlayed = 0;
        }

        if (ctrlData.Buttons & PSP_CTRL_CROSS) {
            loop = !loop;
            status = sceMp3SetLoopNum(handle, loop);
            if (status < 0) {
                ERRORMSG("ERROR: sceMp3SetLoopNum returned 0x%08X\n", status);
            }
        }

        if (ctrlData.Buttons & PSP_CTRL_SQUARE) {
            return 1;
        }
    }
    return 0;
}

void CleanUpAudio() {
    // Free channel
    if (channel >= 0) {
        sceAudioSRCChRelease();
    }

    // Release MP3 handle
    status = sceMp3ReleaseMp3Handle(handle);
    if (status < 0) {
        ERRORMSG("ERROR: sceMp3ReleaseMp3Handle returned 0x%08X\n", status);
    }

    status = sceMp3TermResource();
    if (status < 0) {
        ERRORMSG("ERROR: sceMp3TermResource returned 0x%08X\n", status);
    }

    status = sceIoClose(fd);
    if (status < 0) {
        ERRORMSG("ERROR: sceIoClose returned 0x%08X\n", status);
    }
}