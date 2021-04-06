//
// Created by kosmas on 28/1/21.
//

#define MAININCLUDE 1
#include "system_PSP.h"
#undef MAININCLUDE
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspaudio.h>
#include <pspmp3.h>
#include <psputility.h>
#include <pspdisplay.h>
#include "player_PSP_pspaudio.h"
#include "sourcePicker_PSP_sceio.h"

PSP_MODULE_INFO("XMusic", 0, 0, 1);
PSP_HEAP_SIZE_MAX();

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    exitted = true;
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

void error(char *msg)
{
    SceCtrlData pad;
    pspDebugScreenClear();
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf(msg);
    pspDebugScreenPrintf("Press X to quit.\n");

    while (true) {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_CROSS)
            break;
        sceDisplayWaitVblankStart();
    }
    sceKernelExitGame();
}

void Init() {
    SetupCallbacks();
    pspDebugScreenInit();
    pspDebugScreenPrintf("XMusic, a music player.\n");
    shouldStop = true;
    exitted = false;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    status = sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
    if (status < 0) {
        ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC) returned 0x%08X\n", status);
    }

    status = sceUtilityLoadModule(PSP_MODULE_AV_MP3);
    if (status < 0) {
        ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_MP3) returned 0x%08X\n", status);
    }
}

void FreeMusic() {
    return;
}

void ClearScreen() {
    pspDebugScreenClear();
}

void UpdateScreen() {
    return;
}