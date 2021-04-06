//
// Created by kosmas on 28/1/21.
//

#pragma once

//#define printf pspDebugScreenPrintf

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

#include <pspkernel.h>
#include <pspctrl.h>
#include <string>

#define ERRORMSG(...) { char msg[128]; sprintf(msg,__VA_ARGS__); error(msg); }

EXTERN bool exitted;
EXTERN std::string pathToPlay;
EXTERN SceCtrlData ctrlData;
EXTERN SceCtrlData oldCtrlData;
EXTERN int status;
EXTERN int fd;
EXTERN int channel;
EXTERN int samplingRate;
EXTERN int numChannels;
EXTERN int lastDecoded;
EXTERN int volume;
EXTERN int numPlayed;
EXTERN int paused;
EXTERN int loop;
EXTERN int handle;

void error(char *msg);
void Init();
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
void FreeMusic();
void ClearScreen();
void UpdateScreen();

