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
#include <algorithm>
#include "lib/FilesystemX/FilesystemX.hpp"
std::string fileToPlay = "";
SDL_GameController *controller = NULL;
SDL_Rect pos = {0, 0, 0, 0};
SDL_Surface *text;
SDL_Window* window = NULL;


#if defined(NXDK)
#define ROOT "D:\\"
#else
#define ROOT "./"
#endif
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
  Mix_FreeMusic(music);
  Mix_CloseAudio();
  SDL_DestroyWindow(window);
  SDL_GameControllerClose(controller);
  SDL_Quit();
  Mix_Quit();
  IMG_Quit();
  TTF_CloseFont(Roboto);
  TTF_Quit();
  //SDL_FreeWAV(wavBuffer);
  #if defined (NXDK)
  XReboot();
  #endif
  exit(exitcode);
}

void Draw(SDL_Surface *borderImage, SDL_Surface *arrowImage, SDL_Window *window, std::vector<ProtoFS::fileEntry> listDir) {
    SDL_BlitSurface(borderImage, NULL, SDL_GetWindowSurface(window), NULL);
    pos = {95, 20, 500, 20};
    for (int i = 0; i < std::min((int)listDir.size(), (int) 20); i++) {
        text = TTF_RenderText_Blended(Roboto, listDir[i].fileName.c_str(), color);
        pos.y += 20;
        SDL_BlitSurface(text, NULL, SDL_GetWindowSurface(window), &pos);
        SDL_FreeSurface(text);
        if (i == (curSelection % 20)) {
            pos.x = 45;
            SDL_BlitSurface(arrowImage, NULL, SDL_GetWindowSurface(window), &pos);
            pos.x = 95;
        }
    
    }
}

int showFilePicker(SDL_Window *window) {
    SDL_Surface* borderImage = IMG_Load(ROOT"border.png");
    SDL_Surface* arrowImage = IMG_Load(ROOT"arrow.png");
    Roboto = TTF_OpenFont(ROOT"Roboto-Regular.ttf", 15);
    #if defined (NXDK) || defined (_WIN32)
    ProtoFS::FilesystemX fs(ROOT"Music\\");
    #else
    ProtoFS::FilesystemX fs(ROOT"Music/");
    #endif
    std::vector<ProtoFS::fileEntry> listDir = fs.listDir();
    while (true) {
        if(Roboto != NULL){
            Draw(borderImage, arrowImage, window, listDir);
            while (SDL_PollEvent(&event)) {
                switch(event.type){
                    case SDL_QUIT:
                        exit(0);
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_UP:
                            if(curSelection > 0) {
                                curSelection--;
                            }
                            else {
                                curSelection = (int)listDir.size() - 1;
                            }
                            break;
                            case SDLK_DOWN:
                                if(curSelection < (int)listDir.size() - 1) {
                                    curSelection++;
                                }
                                else {
                                    curSelection = 0;
                                }
                                break;
                            case SDLK_RETURN:
                                fileToPlay = listDir[curSelection].filePath;
                                return (int)listDir.size();
                            case SDLK_ESCAPE:
                                Quit(music, 0);
                            default:
                                break;
                            }
                        break;
                    case SDL_CONTROLLERBUTTONDOWN:
                        switch(event.cbutton.button){
                            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                                if(curSelection > 0) {
                                    curSelection--;
                                }
                                else {
                                    curSelection = (int)listDir.size() - 1;
                                }
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                if(curSelection < (int)listDir.size() - 1) {
                                    curSelection++;
                                }
                                else {
                                    curSelection = 0;
                                }
                                break;
                            case SDL_CONTROLLER_BUTTON_A:
                                fileToPlay = listDir[curSelection].filePath;
                                return (int)listDir.size();
                            case SDL_CONTROLLER_BUTTON_B:
                                Quit(music, 0);
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
            break;
        }
        SDL_Delay(200);
    }
    return (int)listDir.size();
}
