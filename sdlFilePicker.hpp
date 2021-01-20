/*
SDLFilePicker for XMusic Copyright (C) 2020-2021  Bob Willett
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
#if defined(NXDK)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif
#include <string>
#include <algorithm>
#include <iostream>
#include "lib/FilesystemX/FilesystemX.hpp"

std::string fileToPlay = "";
SDL_GameController *controller = NULL;
SDL_Rect pos = {0, 0, 0, 0};
SDL_Surface *text;
SDL_Window* window = NULL;
int controllerport = 0;
std::string controllername = "";
bool exitted = 0;
SDL_Surface *windowSurface;

void OpenFirstController() {
  //Open controller

  for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

    if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
      controller = SDL_GameControllerOpen(i); // Open the controller
      if(controller) { // If we find that we opened a controller
        controllerport = i;
        controllername = SDL_GameControllerName(controller);
        break; // Exit the loop
      }
    }
  }
}

void CloseFirstController() {
  if (controller != NULL) {
    SDL_GameControllerClose(controller);
    printf("Controller 1 closed\n");
  }
}

#if defined(NXDK)
#define ROOT "D:\\"
#define SEPARATOR "\\"
#elif defined (__WIN32__)
#define ROOT ".\\"
#define SEPARATOR "\\"
#else
#define ROOT "./"
#define SEPARATOR "/"
#endif
std::string curPath = ROOT "Music" SEPARATOR;
SDL_Event event;
TTF_Font *Roboto = NULL;
int curSelection = 0;
SDL_Color color = {255, 255, 255};
Mix_Music *music = NULL;

int InitFilePicker() {
  int ret = TTF_Init();
  ret += IMG_Init(IMG_INIT_PNG);
  return ret;
}

void Quit(Mix_Music *music, int exitcode) {
  Mix_CloseAudio();
  SDL_DestroyWindow(window);
  CloseFirstController();
  //SDL_FreeWAV(wavBuffer);
  Mix_Quit();
  IMG_Quit();
  TTF_CloseFont(Roboto);
  TTF_Quit();
  SDL_Quit();
  #if defined (NXDK)
  XReboot();
  #else
  exit(exitcode);
  #endif
}

void Draw(SDL_Surface *borderImage, SDL_Surface *arrowImage, SDL_Window *window, std::vector<ProtoFS::fileEntry> listDir) {
  SDL_BlitSurface(borderImage, NULL, windowSurface, NULL);
  pos = {95, 20, 500, 20};
  for (int i = 0; i < std::min((int)listDir.size(), (int) 20); i++) {
    text = TTF_RenderText_Blended(Roboto, listDir[i].fileName.c_str(), color);
    pos.y += 20;
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    SDL_FreeSurface(text);
    if (i == (curSelection % 20)) {
      pos.x = 45;
      SDL_BlitSurface(arrowImage, NULL, windowSurface, &pos);
      pos.x = 95;
    }
  }
}

int showFilePicker(SDL_Window *window) {
  SDL_Surface* borderImage = IMG_Load(ROOT"border.png");
  SDL_Surface* arrowImage = IMG_Load(ROOT"arrow.png");
  Roboto = TTF_OpenFont(ROOT"Roboto-Regular.ttf", 15);
  ProtoFS::FilesystemX *fs;
  fs = new ProtoFS::FilesystemX(curPath);
  std::vector<ProtoFS::fileEntry> listDir = fs->listDir();
  while (true) {
    if(Roboto != NULL){
      Draw(borderImage, arrowImage, window, listDir);
      while (SDL_PollEvent(&event)) {
        switch(event.type) {
          case SDL_QUIT:
            exitted = 1;
            Quit(music, 0);
          case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
              case SDLK_UP:
                if (curSelection > 0) {
                  curSelection--;
                } else {
                  curSelection = (int) listDir.size() - 1;
                }
                break;
              case SDLK_DOWN:
                if (curSelection < (int) listDir.size() - 1) {
                  curSelection++;
                } else {
                  curSelection = 0;
                }
                break;
              case SDLK_RETURN:
                if (listDir[curSelection].type == ProtoFS::fileType::Folder) {
                  curPath = curPath + listDir[curSelection].fileName + SEPARATOR;
                  delete fs;
                  fs = new ProtoFS::FilesystemX(curPath);
                  listDir = fs->listDir();
                }
                else {
                  fileToPlay = listDir[curSelection].filePath;
                  return (int) listDir.size();
                }
                break;
              case SDLK_ESCAPE:
                exitted = 1;
                Quit(music, 0);
                break;
              case SDLK_r:
                listDir = fs->listDir();
                break;
              case SDLK_BACKSPACE:
                curPath = curPath + ".." + SEPARATOR;
                delete fs;
                fs = new ProtoFS::FilesystemX(curPath);
                listDir = fs->listDir();
                break;
            }
            break;
          case SDL_CONTROLLERBUTTONDOWN:
            switch (event.cbutton.button) {
              case SDL_CONTROLLER_BUTTON_DPAD_UP:
                if (curSelection > 0) {
                  curSelection--;
                } else {
                  curSelection = (int) listDir.size() - 1;
                }
                break;
              case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                if (curSelection < (int) listDir.size() - 1) {
                  curSelection++;
                } else {
                  curSelection = 0;
                }
                break;
              case SDL_CONTROLLER_BUTTON_A:
                fileToPlay = listDir[curSelection].filePath;
                return (int) listDir.size();
              case SDL_CONTROLLER_BUTTON_B:
                exitted = 1;
                delete fs;
                Quit(music, 0);
              case SDL_CONTROLLER_BUTTON_Y:
                listDir = fs->listDir();
                break;
              default:
                break;
            }
            break;
          default:
            break;
        }
      }
      SDL_UpdateWindowSurface(window);
    }
    else {
      printf("Couldn't initialize font. Reason: %s", TTF_GetError());
      SDL_Delay(200);
      break;
    }
  }
  return (int)listDir.size();
}
