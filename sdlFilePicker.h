/*
SDLFilePicker for XMusic Copyright (C) 2020  Bob Willett
Email: bobrow2006@gmail.com

XMusic is copyrighted by Kosmas Raptis

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
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <windows.h>
#include <fileapi.h>

//file struct and GetFiles function "borrowed" (nabbed) from player.c and thus is under copyright by Kosmas Raptir
#define NUMFILES 50
typedef struct
{
  int fileIndex;
  char fileName[50];
  char filePath[150];
}file;

file * GetFiles(char* driveletter) {
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  file foundFiles[NUMFILES] = {NULL};

  size_t currentFileDirCount = 0;

  char* driveWav;
  sprintf(driveWav, "%s\\.wav", driveletter);

  hFind = FindFirstFileA(driveWav, &findFileData);

  do {
    XVideoWaitForVBlank();
    debugClearScreen();
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    }
    else {
      if (currentFileDirCount < NUMFILES) {
        foundFiles[currentFileDirCount].fileIndex = currentFileDirCount;
        strcpy(foundFiles[currentFileDirCount].fileName, findFileData.cFileName);
        sprintf(foundFiles[currentFileDirCount].filePath, "%s\\%s", driveletter, foundFiles[currentFileDirCount].fileName);
        currentFileDirCount++;
      }
    }
  }
  while (FindNextFileA(hFind, &findFileData) != 0);
  FindClose(hFind);

  return foundFiles;
}
/*
void listFiles(const file files[]) {
  for (int i = 0; i < NUMFILES; i++) {
    if (files[i] != NULL) {
      printf("%d ", files[i].fileIndex);
      printf("%s\n", files[i].filePath);
    }
    else {
      return;
    }
  }
}


static int FileBrowser() {

  file files[50] = GetFiles("D:");

  int currentIndex = 0;

  while (true)
  {
    XVideoWaitForVBlank();
    SDL_GameControllerUpdate();
    debugClearScreen();

    listFiles(files);

    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
      if (currentIndex != 0) {
        currentIndex--;
      }
      else {
        currentIndex = 0;
      }
    }
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
      if (currentIndex != NUMFILES - 1) {
        currentIndex++;
      }
      else {
        currentIndex = NUMFILES - 1;
      }
    }


    printf("\nYour current selected file is: %s (Index number %d)\n", files[currentIndex].fileName, currentIndex);
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
      strcpy(fileToPlay, files[currentIndex].filePath);
      free(files);
      break;
    }
  }

  return 0;
}
 */

bool InitFilePicker() {
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
}

void DrawStaticCrap(SDL_Surface* borderImage, SDL_Window* window) {
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(borderImage,NULL,windowSurface,NULL);

}

char[210] showFilePicker(SDL_Window* window) {
    SDL_Surface* borderImage = SDL_LoadPNG("D:\\border.png");
    TTF_Font* Roboto = TTF_OpenFont("D:\\Roboto-Regular.ttf", int ptsize);
}
