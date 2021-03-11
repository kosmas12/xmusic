/*
XMusic Copyright (C) 2020-2021  Kosmas Raptis
Email: keeperkosmas6@gmail.com

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

#pragma once

#if defined(NXDK)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#endif
#include <string>
#include <sstream>

#if defined(NXDK) || defined(__WIN32__)
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

#define WIDTH 640
#define HEIGHT 480
#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN std::string filesToPlay[250];
EXTERN std::string fileToPlay;
EXTERN int curFileNum;
EXTERN int numFiles;
EXTERN SDL_GameController *controller;
EXTERN SDL_Window* window;
EXTERN int controllerport;
EXTERN std::string controllername;
EXTERN bool exitted;
EXTERN SDL_Surface *windowSurface;
EXTERN SDL_Event event;
EXTERN Mix_Music *music;
EXTERN SDL_Color color;
EXTERN TTF_Font *Roboto;
EXTERN SDL_Rect pos;
EXTERN SDL_Surface *text;
EXTERN bool shouldLoop;
EXTERN bool shouldStop;
EXTERN bool isPlaylist;


void Quit(Mix_Music *music, int exitcode);
void OpenFirstController();
void CloseFirstController();
int InitSourcePicker();
void Init();
void FreeMusic(Mix_Music *music);
void ClearScreen();
void UpdateScreen();


