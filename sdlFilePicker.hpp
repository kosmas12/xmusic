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
#include <stdbool.h>
#include "lib/FilesystemX/FilesystemX.hpp"
#if defined(NXDK)
#define ROOT "D:\\"
#else
#define ROOT "./"
#endif
SDL_Event event;
TTF_Font* Roboto;
SDL_Surface* borderImage;

int InitFilePicker() {
    int ret = TTF_Init();
    ret+=IMG_Init(IMG_INIT_PNG);
    return ret;
}

void DrawStaticCrap(SDL_Surface* borderImage, SDL_Window* window) {
    SDL_Surface* text;
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(borderImage,NULL,windowSurface,NULL);
    ProtoFS::FilesystemX fs(ROOT);
    std::vector<ProtoFS::fileEntry> listDir = fs.listDir();
    SDL_Color color = {255, 255, 255};
    for(int i = 0; i < listDir.size(); i++) {
      text = TTF_RenderText_Solid(Roboto, listDir[i].fileName.c_str(), color);
      SDL_BlitSurface(text, NULL, windowSurface, NULL);
      SDL_UpdateWindowSurface(window);
    }
}

char* showFilePicker(SDL_Window* window) {
    borderImage = IMG_Load(ROOT"border.png");
    Roboto = TTF_OpenFont(ROOT"Roboto-Regular.ttf", 25);
    while (true) {
        while (SDL_PollEvent(&event))
        {
          if(event.type == SDL_QUIT) {
            exit(0);
          }
        }

        DrawStaticCrap(borderImage, window);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2000);
    }
}
