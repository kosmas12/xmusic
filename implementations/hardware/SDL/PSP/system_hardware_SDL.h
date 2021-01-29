//
// Created by kosmas on 28/1/21.
//

#pragma once

#define printf pspDebugScreenPrintf

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

#include <pspkernel.h>
#include <pspctrl.h>
#include <SDL2/SDL_mixer.h>

EXTERN bool exitted;
EXTERN const char *fileToPlay;
EXTERN SceCtrlData ctrlData;
EXTERN SceCtrlData oldCtrlData;

void Init();
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
void FreeMusic(Mix_Music *music);
void ClearScreen();
void UpdateScreen();

