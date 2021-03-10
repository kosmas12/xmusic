/*
XMusic Copyright (C) 2020-2021  Kosmas Raptis
Email: keeperkosmas6@gmail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/

#define MAININCLUDE 1
#include "player_software_SDL_mixer.h"
#include "system_software_SDL.h"
#include <cstdio>
#include <fstream>

std::stringstream formatString;

void LoadPlaylist() {
    std::ifstream fp(filesToPlay[0]);
    std::string curLine;

    numFiles = 0;

    while (std::getline(fp, curLine)) {
        if (numFiles < 200) {
            filesToPlay[numFiles] = curLine;
            numFiles++;
        }
        else {
            break;
        }
    }
    fp.close();
}

bool GetNextInPlaylist() {
    if (curFileNum <= numFiles) {
        fileToPlay = filesToPlay[curFileNum];
        return true;
    }
    return false;
}

void PutToWindow(std::string string, TTF_Font* font, SDL_Rect *pos) {
    text = TTF_RenderUTF8_Blended(font, string.c_str(), color);
    SDL_BlitSurface(text, NULL, windowSurface, pos);
    SDL_FreeSurface(text);
    pos->y += 20;
    SDL_UpdateWindowSurface(window);
    formatString.str("");
}

void UpdateVolume() {
    SDL_Rect volumePos = {130, 5, 150, 20};
    SDL_FillRect(windowSurface, &volumePos, SDL_MapRGB(windowSurface->format, 0, 0, 0));
    formatString << "Volume: " << audio_volume;
    PutToWindow(formatString.str(), Roboto, &volumePos);
}

void UpdateLoop() {
    SDL_Rect loopPos = {280, 5, 150, 20};
    SDL_FillRect(windowSurface, &loopPos, SDL_MapRGB(windowSurface->format, 0, 0, 0));
    std::string toWrite;
    if (shouldLoop) {
        toWrite = "Loop: on";
    }
    else {
        toWrite = "Loop: off";
    }
    PutToWindow(toWrite, Roboto, &loopPos);
}

void UpdatePause() {
    SDL_Rect pausePos = {430, 5, 150, 20};
    SDL_FillRect(windowSurface, &pausePos, SDL_MapRGB(windowSurface->format, 0, 0, 0));
    std::string toWrite;
    if (paused) {
        toWrite = "Paused";
    }
    else {
        toWrite = "Playing";
    }
    PutToWindow(toWrite, Roboto, &pausePos);
}

void IncreaseVolume() {
    if (audio_volume + 2 < MIX_MAX_VOLUME) {
        audio_volume += 2;
    }
    else {
        audio_volume = MIX_MAX_VOLUME;
    }
    Mix_VolumeMusic(audio_volume);
    UpdateVolume();
}

void DecreaseVolume() {
    if (audio_volume - 2 > 0) {
        audio_volume -= 2;
    }
    else {
        audio_volume = 0;
    }
    Mix_VolumeMusic(audio_volume);
    UpdateVolume();
}

void ToggleLoop() {
    shouldLoop = !shouldLoop;
    if (!isPlaylist) {
        shouldStop = !shouldLoop;
    }
    UpdateLoop();
}

void TogglePause() {
    if(paused == 0) {
        //SDL_PauseAudioDevice(deviceID, 1);
        Mix_PauseMusic();
    }
    else {
        //SDL_PauseAudioDevice(deviceID, 0);
        Mix_ResumeMusic();
    }
    paused = !paused;
    UpdatePause();
}

void StopPlaying() {
    Mix_HaltMusic();
    shouldLoop = false;
    shouldStop = true;
}

void PlaySource() {

    // The playlist hasn't been loaded yet
    if (isPlaylist && numFiles == 0) {
        LoadPlaylist();
        shouldStop = false;
    }

    if (isPlaylist && !shouldLoop) {
        GetNextInPlaylist();
        curFileNum++;
    }
    else if (isPlaylist && shouldLoop){
        fileToPlay = filesToPlay[curFileNum-1];
    }
    else {
        fileToPlay = filesToPlay[0];
    }

    audio_volume = MIX_MAX_VOLUME;
    paused = 0;
    pos = {45, 40, 500, 20};

    int audio_rate = MIX_DEFAULT_FREQUENCY; // On Xbox, this is currently ignored and audio always opens at 48KHz
    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = MIX_DEFAULT_CHANNELS;
    int audio_buffers = 4096;
    int looping = 1;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
        formatString << "Couldn't open audio: " << SDL_GetError();
        PutToWindow(formatString.str(), Roboto, &pos);
    }
    else {
        Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
        formatString << "Opened audio at " << audio_rate << " Hz " << (audio_format&0xFF) <<  " bit " << (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : "")
                     <<  ((audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo " : "mono ") <<  audio_buffers << " bytes audio buffer";
        PutToWindow(formatString.str(), Roboto, &pos);
    }
    audio_open = 1;

    formatString << "Setting volume to " << audio_volume;
    PutToWindow(formatString.str(), Roboto, &pos);
    Mix_VolumeMusic(audio_volume);
    formatString << "Opening " << fileToPlay;
    PutToWindow(formatString.str(), Roboto, &pos);
    SDL_RWops *rw = SDL_RWFromFile(fileToPlay.c_str(), "rb");

    if (rw == NULL) {
        formatString << "Couldn't open " << fileToPlay << ": " << Mix_GetError();
        PutToWindow(formatString.str(), Roboto, &pos);
        SDL_RWclose(rw);
        Quit(music, 2);
    }
    formatString << "Loading " << fileToPlay;
    PutToWindow(formatString.str(), Roboto, &pos);
    music = Mix_LoadMUS_RW(rw, 1);
    if (music == NULL) {
        formatString << "Couldn't load " << fileToPlay << ": " << Mix_GetError();
        PutToWindow(formatString.str(), Roboto, &pos);
        SDL_RWclose(rw);
        Quit(music, 3);
    }
    formatString << "Loaded " << fileToPlay;
    PutToWindow(formatString.str(), Roboto, &pos);
    Mix_PlayMusic(music, looping);

    if (controller != NULL) {
        formatString << "Opened controller " << controllername << " on port " << controllerport;
        PutToWindow(formatString.str(), Roboto, &pos);
    }
    std::string nameToShow = fileToPlay.substr(fileToPlay.rfind(SEPARATOR) + 1);
    formatString << "Now playing: " << nameToShow;
    PutToWindow(formatString.str(), Roboto, &pos);

    UpdateVolume();
    UpdateLoop();
    UpdatePause();
}

void ProcessInput() {
    while (SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_CONTROLLERDEVICEADDED:
                OpenFirstController();
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                CloseFirstController();
                break;
            case SDL_QUIT:
                Quit(music, 0);
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_AUDIOPLAY:
                    case SDLK_SPACE:
                        TogglePause();
                        break;
                    case SDLK_ESCAPE:
                        StopPlaying();
                        break;
                    case SDLK_DOWN:
                    case SDLK_VOLUMEDOWN:
                        DecreaseVolume();
                        break;
                    case SDLK_UP:
                    case SDLK_VOLUMEUP:
                        IncreaseVolume();
                        break;
                    case SDLK_l:
                        ToggleLoop();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                switch(event.cbutton.button){
                    case SDL_CONTROLLER_BUTTON_A:
                        TogglePause();
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        StopPlaying();
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        DecreaseVolume();
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        IncreaseVolume();
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        ToggleLoop();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

bool isPlaying() {
    return Mix_PlayingMusic() || Mix_PausedMusic();
}