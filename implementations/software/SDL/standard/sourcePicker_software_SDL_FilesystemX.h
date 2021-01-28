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