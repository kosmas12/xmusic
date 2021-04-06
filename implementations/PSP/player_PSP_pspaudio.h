//
// Created by kosmas on 28/1/21.
//

#pragma once

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN bool shouldStop;
EXTERN char	mp3Buf[16*1024]  __attribute__((aligned(64)));
EXTERN short pcmBuf[16*(1152/2)]  __attribute__((aligned(64)));
EXTERN int bytesDecoded;

bool isPlaying();
int ProcessInput();
void PlaySource();
void CleanUpAudio();
