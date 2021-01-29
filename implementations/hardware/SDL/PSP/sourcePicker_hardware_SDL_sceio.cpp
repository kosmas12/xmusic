//
// Created by kosmas on 28/1/21.
//

#include "sourcePicker_hardware_SDL_sceio.h"
#include "system_hardware_SDL.h"
#include <pspiofilemgr.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>

void selectSource() {
    SceIoDirent dir;
    memset(&dir, 0, sizeof(SceIoDirent));
    SceUID fd;
    const char *basedir = "host0:/Music";
    bool fileSelected = false;
    char *files[20]= {nullptr};
    int fileCount = 0;

    fd = sceIoDopen(basedir);
    if (fd >= 0) {
        while (sceIoDread(fd, &dir) > 0) {
            if ((dir.d_stat.st_attr & FIO_SO_IFDIR) == 0) {
                files[fileCount] = (char*) malloc (strlen(dir.d_name)+1);
                strncpy (files[fileCount], dir.d_name, strlen(dir.d_name) );
                fileCount++;
            }
        }
        sceIoDclose(fd);
    }
    int index = 0;

    printf("\nFound files in the Music directory: \n\n");

    for (int i = 0; i < fileCount; i++) {
        printf("%d: %s\n", i, files[i]);
    }

    while (!fileSelected) {

        oldCtrlData = ctrlData;
        sceCtrlReadBufferPositive(&ctrlData, 1);
        if (ctrlData.Buttons & PSP_CTRL_CROSS && ctrlData.Buttons != oldCtrlData.Buttons) {
            fileSelected = true;
        }
        if (ctrlData.Buttons & PSP_CTRL_DOWN && ctrlData.Buttons != oldCtrlData.Buttons) {
            if (index < fileCount-1) {
                index++;
                printf("\nCurrently selected file: %s", files[index]);
            }
        }
        if (ctrlData.Buttons & PSP_CTRL_UP && ctrlData.Buttons != oldCtrlData.Buttons) {
            if (index > 0) {
                index--;
                printf("\nCurrently selected file: %s", files[index]);
            }
        }
    }
}
