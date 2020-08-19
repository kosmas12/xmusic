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
std::string fileToPlay;
SDL_GameController *controller = NULL;


#if defined(NXDK)
#define ROOT "D:\\"
#else
#define ROOT "./"
#endif
SDL_Event event;
TTF_Font *Roboto = NULL;
int curSelection = 0;

int InitFilePicker() {
    int ret = TTF_Init();
    ret += IMG_Init(IMG_INIT_PNG);
    return ret;
}

void Draw(SDL_Surface *borderImage, SDL_Surface *arrowImage, SDL_Window *window, std::vector<ProtoFS::fileEntry> listDir) {
    SDL_Surface *text = NULL;
    SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(borderImage, NULL, windowSurface, NULL);
    SDL_Color color = {255, 255, 255};
    SDL_Rect pos = {95, 20, 500, 20};
    for (int i = 0; i < (int)listDir.size(); i++) {
        text = TTF_RenderText_Blended(Roboto, listDir[i].fileName.c_str(), color);
        pos.y += 20;
        SDL_BlitSurface(text, NULL, windowSurface, &pos);
        if (i == (curSelection % 20)) {
            pos.x = 45;
            SDL_BlitSurface(arrowImage, NULL, windowSurface, &pos);
            pos.x = 95;
        }
    
    }
}

void showFilePicker(SDL_Window *window) {
    SDL_Surface* borderImage = IMG_Load(ROOT"border.png");
    SDL_Surface* arrowImage = IMG_Load(ROOT"arrow.png");
    Roboto = TTF_OpenFont(ROOT"Roboto-Regular.ttf", 15);
    #if defined (NXDK) || defined (_WIN32)
    ProtoFS::FilesystemX fs(ROOT"Music\\*.*");
    #else
    ProtoFS::FilesystemX fs(ROOT"Music/");
    #endif
    std::vector<ProtoFS::fileEntry> listDir = fs.listDir();
    while (true) {
        if(Roboto != NULL){
            Draw(borderImage, arrowImage, window, listDir);
            #ifndef NXDK //Events are bad for performance on Xbox, so instead we will use SDL_GameController functions
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
                                curSelection = (int)listDir.size();
                            }
                            break;
                        case SDLK_DOWN:
                            if(curSelection < (int)listDir.size()) {
                                curSelection++;
                            }
                            else {
                                curSelection = 0;
                            }
                            break;
                        case SDLK_RETURN:
                            fileToPlay = listDir[curSelection].filePath;
                            return;
                        default:
                            break;
                        }
                        break;
                case SDL_CONTROLLERBUTTONDOWN:
                    switch(event.cbutton.button){
                        case SDL_CONTROLLER_BUTTON_DPAD_UP:
                            curSelection++;
                            break;
                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                            curSelection--;
                            break;
                        case SDL_CONTROLLER_BUTTON_A:
                            fileToPlay = listDir[curSelection].filePath;
                            return;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
            #else
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
                curSelection++;
            }
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
                curSelection--;
            }
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)){
                fileToPlay = listDir[curSelection].filePath;
                return;
            }
            #endif
            SDL_UpdateWindowSurface(window);
        }
        else {
            printf("Couldn't initialize font. Reason: %s", TTF_GetError());
            break;
        }
        SDL_Delay(200);
    }
}
