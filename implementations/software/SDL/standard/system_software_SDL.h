//
// Created by kosmas on 27/1/21.
//

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

#define WIDTH 640
#define HEIGHT 480
#ifdef MAININCLUDE
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN std::string fileToPlay;
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


void Quit(Mix_Music *music, int exitcode);
void OpenFirstController();
void CloseFirstController();
int InitSourcePicker();
void Init();
void FreeMusic(Mix_Music *music);
void ClearScreen();
void UpdateScreen();


