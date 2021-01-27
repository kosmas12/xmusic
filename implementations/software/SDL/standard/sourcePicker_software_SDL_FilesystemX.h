//
// Created by kosmas on 27/1/21.
//

#pragma once

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
#include "../../../../lib/FilesystemX/FilesystemX.hpp"
#include "system_software_SDL.h"

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

#ifdef MAININCLUDE
#ifndef EXTERN
#define EXTERN
#endif
#else
#ifndef EXTERN
#define EXTERN extern
#endif
#endif

EXTERN std::string curPath;
EXTERN int curSelection;

void Draw(SDL_Surface *borderImage, SDL_Surface *arrowImage, SDL_Window *window, std::vector<ProtoFS::fileEntry> listDir);
int showSourcePicker(SDL_Window *window);
void selectSource();