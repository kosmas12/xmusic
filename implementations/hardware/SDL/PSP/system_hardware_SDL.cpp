//
// Created by kosmas on 28/1/21.
//

#define MAININCLUDE 1
#include "system_hardware_SDL.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include "player_hardware_SDL_mixer.h"
#include "sourcePicker_hardware_SDL_sceio.h"

PSP_MODULE_INFO("XMusic", 0, 1, 1);
PSP_HEAP_SIZE_MAX();

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }
    return thid;
}


void Init() {
    SetupCallbacks();
    pspDebugScreenInit();
    pspDebugScreenPrintf("XMusic, a music player.\n");
    shouldLoop = false;
    exitted = false;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void FreeMusic(Mix_Music *music) {
    return;
}

void ClearScreen() {
    return;
}

void UpdateScreen() {
    return;
}