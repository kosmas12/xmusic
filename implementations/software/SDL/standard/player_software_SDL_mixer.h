//
// Created by kosmas on 26/1/21.
//Zz

#pragma once

#if defined(NXDK)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif
#include <string>

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN int audio_volume;
EXTERN int paused;
EXTERN int audio_open;



void UpdateVolume();
void UpdateLoop();
void UpdatePause();
void PutToWindow(std::string string, TTF_Font* font, SDL_Rect *pos);
void PlaySource();
void ProcessInput();
bool isPlaying();

