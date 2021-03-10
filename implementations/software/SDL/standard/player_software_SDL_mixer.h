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
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif
#include <string>

#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN int audio_volume;
EXTERN int paused;
EXTERN int audio_open;


void LoadPlaylist();
bool GetNextInPlaylist();
void UpdateVolume();
void UpdateLoop();
void UpdatePause();
void PutToWindow(std::string string, TTF_Font* font, SDL_Rect *pos);
void PlaySource();
void ProcessInput();
bool isPlaying();

