//
// Created by kosmas on 28/1/21.
//

#include "system_PSP.h"
#include "sourcePicker_PSP_sceio.h"
#include <pspiofilemgr.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <cstdlib>

void selectSource() {
    SceIoDirent dir;
    SceUID fd;
    const std::string basedir = "ms0:/MUSIC/";
    bool fileSelected = false;
    std::string files[20] = {""};
    int fileCount = 0;

    fd = sceIoDopen(basedir.c_str());
    if (fd >= 0) {
        while (sceIoDread(fd, &dir) > 0) {
            if ((dir.d_stat.st_attr & FIO_SO_IFDIR) == 0) {
                files[fileCount] = dir.d_name;
                fileCount++;
            }
        }
        sceIoDclose(fd);
    }
    int index = 0;

    pspDebugScreenPrintf("\nFound files in the %s directory: \n\n", basedir.c_str());

    for (int i = 0; i < fileCount; i++) {
        pspDebugScreenPrintf("%d: %s\n", i, files[i].c_str());
    }

    pspDebugScreenSetXY(0, fileCount+4);
    pspDebugScreenPrintf("\nCurrently selected file: %s\n", files[index].c_str());

    while (!fileSelected) {
        oldCtrlData = ctrlData;
        sceCtrlReadBufferPositive(&ctrlData, 1);
        pspDebugScreenSetXY(0, fileCount+4);
        if (ctrlData.Buttons & PSP_CTRL_CROSS && ctrlData.Buttons != oldCtrlData.Buttons) {
            pathToPlay = basedir + files[index];
            fileSelected = true;
        }
        if (ctrlData.Buttons & PSP_CTRL_DOWN && ctrlData.Buttons != oldCtrlData.Buttons) {
            if (index < fileCount-1) {
                index++;
                pspDebugScreenPrintf("\nCurrently selected file: %s\n", files[index].c_str());
            }
        }
        if (ctrlData.Buttons & PSP_CTRL_UP && ctrlData.Buttons != oldCtrlData.Buttons) {
            if (index > 0) {
                index--;
                pspDebugScreenPrintf("\nCurrently selected file: %s\n", files[index].c_str());
            }
        }
    }
}