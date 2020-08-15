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

#if defined(NXDK)
#define ROOT "D:\\"
#else
#define ROOT "../"
#endif
SDL_Event event;
TTF_Font *Roboto;
int curSelection = 0;

int InitFilePicker() {
    int ret = TTF_Init();
    ret += IMG_Init(IMG_INIT_PNG);
    return ret;
}

void Draw(SDL_Surface *borderImage, SDL_Surface *arrowImage, SDL_Window *window) {
    SDL_Surface *text;
    SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(borderImage, NULL, windowSurface, NULL);
    ProtoFS::FilesystemX fs(ROOT);
    std::vector <ProtoFS::fileEntry> listDir = fs.listDir();
    SDL_Color color = {255, 255, 255};
    SDL_Rect pos = {95, 20, 500, 20};
    for (int i = 0; i < std::min(listDir.size(), (long unsigned int) 20); i++) {
        text = TTF_RenderText_Blended(Roboto, listDir[i].fileName.c_str(), color);
        pos.y += 20;
        SDL_BlitSurface(text, NULL, windowSurface, &pos);
        if (i == (curSelection % 21)) {
            pos.x = 45;
            SDL_BlitSurface(arrowImage, NULL, windowSurface, &pos);
            pos.x = 95;
        }
    }
}

char *showFilePicker(SDL_Window *window) {
    SDL_Surface* borderImage = IMG_Load(ROOT"border.png");
    SDL_Surface* arrowImage = IMG_Load(ROOT"arrow.png");
    Roboto = TTF_OpenFont(ROOT"Roboto-Regular.ttf", 15);
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }

        Draw(borderImage, arrowImage, window);
        curSelection++;
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2000);
    }
}
