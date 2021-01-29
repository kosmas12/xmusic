//
// Created by kosmas on 28/1/21.
//

#pragma once

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

#include <SDL2/SDL_mixer.h>

EXTERN Mix_Music *music;

EXTERN bool shouldLoop;

bool isPlaying();
void ProcessInput();
void PlaySource();
