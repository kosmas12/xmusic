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

#include "system_software_SDL.h"

#if defined(NXDK)
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
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

int InitSourcePicker() {
    int ret = TTF_Init();
    ret += IMG_Init(IMG_INIT_PNG);
    return ret;
}

void Init() {

#if defined (NXDK)
    XVideoSetMode(WIDTH, HEIGHT, 32, REFRESH_DEFAULT);
    debugClearScreen();
#endif

    printf("XMusic, Copyright (C) 2020-2021 Kosmas Raptis and Bob Willett\n");
    printf("XMusic comes with ABSOLUTELY NO WARRANTY; ");
    printf("This is free software, and you are welcome to redistribute it under certain conditions; ");
    printf("Read the GNU General Public License v2 for details.\n\n");
    printf("Source code for the specific version you're using came with this executable.\n\n");

    SDL_Delay(3000);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    shouldLoop = false;
    shouldStop = true;
    exitted = false;
    color = {255, 255, 255};
    pos = {0, 0, 0, 0};
    isPlaylist = false;
    curFileNum = 0;
    int mixflags = MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MID|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OPUS;
    int mixinitted = Mix_Init(mixflags);
    printf("Return value of Mix_Init(): %d\n", mixinitted);
    window = SDL_CreateWindow( "XMusic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
    windowSurface = SDL_GetWindowSurface(window);
    int ret = InitSourcePicker();
    if(!ret) {
        printf("File Picker disabled as it was not possible to init SDL_Image and/or SDL_TTF %u",ret);
    }
    SDL_Delay(1500);
    OpenFirstController();
}

void FreeMusic(Mix_Music *music) {
    Mix_FreeMusic(music);
}

void ClearScreen() {
    pos = {45, 40, 560, 200};
    SDL_FillRect(windowSurface, &pos, SDL_MapRGB(windowSurface->format, 0, 0, 0));
}

void UpdateScreen() {
    SDL_UpdateWindowSurface(window);
}