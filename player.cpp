/*
XMusic Copyright (C) 2020  Kosmas Raptis
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


#define SDL_MAIN_HANDLED
#if defined (NXDK)
#include <string.h>
#include <hal/video.h>
#include <windows.h>
#include <fileapi.h>
#include <hal/xbox.h>
#include <hal/debug.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <nxdk/mount.h>
#define printf(...) debugPrint(__VA_ARGS__)
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif
#include "sdlFilePicker.hpp"
#include <cstdio>
#include <sstream>

//static Uint8 *audio_position = NULL; 
//static Uint32 audio_length = 0;
//Uint8 *wavBuffer = NULL;
static int audio_open = 0;
std::stringstream formatString;
int audio_volume = MIX_MAX_VOLUME;
int paused = 0;


SDL_AudioDeviceID deviceID = 0;

/*
void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_length == 0)
		return;
	
	len = (len > audio_length ? audio_length : len);
	SDL_memcpy(stream, audio_position, len); // simply copy from one buffer into the other
	
	audio_position += len;
	audio_length -= len;
} */

static void PutToWindow(std::string string, TTF_Font* font) {
  text = TTF_RenderText_Blended(font, string.c_str(), color);
  SDL_BlitSurface(text, NULL, SDL_GetWindowSurface(window), &pos);
  SDL_FreeSurface(text);
  pos.y += 15;
  SDL_UpdateWindowSurface(window);
  formatString.str("");
} 

static void PlayFile() {

  pos = {45, 40, 500, 20};

  /*
  //Uint32 wavLength;
  //SDL_AudioSpec wavSpec;

  SDL_LoadWAV(fileToPlay.c_str(), &wavSpec, &wavBuffer, &wavLength);//FIXME: Ask for file at runtime
  wavSpec.callback = audio_callback;
	wavSpec.userdata = NULL;

  audio_position = wavBuffer;
	audio_length = wavLength;
    //deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default
  */
 


#if defined(NXDK)
    int audio_rate = 48000; //48KHz saves CPU time
    Uint16 audio_format = AUDIO_S16LSB;
    int audio_channels = 2;
#else
    int audio_rate = MIX_DEFAULT_FREQUENCY;
    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = MIX_DEFAULT_CHANNELS;
#endif 
    int audio_buffers = 4096;
    int looping = 1;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
      formatString << "Couldn't open audio: " << SDL_GetError();
      PutToWindow(formatString.str(), Roboto);
    } 
    else {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      formatString << "Opened audio at " << audio_rate << " Hz " << (audio_format&0xFF) <<  " bit " << (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : "") 
      <<  ((audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo " : "mono ") <<  audio_buffers << " bytes audio buffer";
      PutToWindow(formatString.str(), Roboto);
    }
    audio_open = 1;

    formatString << "Setting volume to " << audio_volume;
    PutToWindow(formatString.str(), Roboto);
    Mix_VolumeMusic(audio_volume);
    formatString << "Opening " << fileToPlay;
    PutToWindow(formatString.str(), Roboto);
    SDL_RWops *rw = SDL_RWFromFile(fileToPlay.c_str(), "rb");

  if (rw == NULL) {
    formatString << "Couldn't open " << fileToPlay << ": " << Mix_GetError();
    PutToWindow(formatString.str(), Roboto);
    Quit(music, 2);
    SDL_RWclose(rw);
  }
  formatString << "Loading " << fileToPlay;
  PutToWindow(formatString.str(), Roboto);
  music = Mix_LoadMUS_RW(rw, SDL_TRUE);
  if (music == NULL) {
    formatString << "Couldn't load " << fileToPlay << ": " << Mix_GetError();
    PutToWindow(formatString.str(), Roboto);
    Quit(music, 3);
    SDL_RWclose(rw);
  }
  formatString << "Loaded " << fileToPlay;
  PutToWindow(formatString.str(), Roboto);
  Mix_PlayMusic(music, looping);
}

static void Init() {

  #if defined (NXDK)
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  debugClearScreen();
  #endif

  printf("XMusic, Copyright (C) 2020 Kosmas Raptis and Bob Willett\n");
  printf("XMusic comes with ABSOLUTELY NO WARRANTY; ");
  printf("This is free software, and you are welcome to redistribute it under certain conditions; ");
  printf("Read the GNU General Public License v2 for details.\n\n");

  SDL_Delay(3000);
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  int mixflags = MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MID|MIX_INIT_MOD|MIX_INIT_MP3/*|MIX_INIT_OPUS*/;
  int mixinitted = Mix_Init(mixflags);
  printf("Return value of Mix_Init(): %d\n", mixinitted);
  window = SDL_CreateWindow( "XMusic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
  int ret = InitFilePicker();
  if(!ret) {
      printf("File Picker disabled as it was not possible to init SDL_Image and/or SDL_TTF %u",ret);
  }
  SDL_Delay(1500);


}

void ProcessInput() {
  SDL_Delay(50); // Delay for a little time to avoid 2 inputs in a small time frame
  while (SDL_PollEvent(&event)) {
    switch(event.type){
      case SDL_QUIT:
        Quit(music, 0);
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_SPACE:
            if(paused == 0) {
              //SDL_PauseAudioDevice(deviceID, 1);
              Mix_PauseMusic();
              paused = 1;
            }
            else {
              Mix_ResumeMusic();
              paused = 0;
            }
            break;
          case SDLK_ESCAPE:
            Mix_HaltMusic();
            break;
          case SDLK_DOWN:
            audio_volume -= 2;
            Mix_VolumeMusic(audio_volume);
            break;
          case SDLK_UP:
            audio_volume += 2;
            Mix_VolumeMusic(audio_volume);
            break;
          default:
            break;
          }
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        switch(event.cbutton.button){
          case SDL_CONTROLLER_BUTTON_B:
            Mix_HaltMusic();
            break;
          case SDL_CONTROLLER_BUTTON_A:
            SDL_Delay(50);
            if(paused == 0) {
              //SDL_PauseAudioDevice(deviceID, 1);
              Mix_PauseMusic();
              paused = 1;
            }
            else {
              Mix_ResumeMusic();
              paused = 0;
            }
            break;
          case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            if (audio_volume > 0) {
              audio_volume -= 2;
            }
            else {
              audio_volume = 0;
            }
            Mix_VolumeMusic(audio_volume);
            break;
          case SDL_CONTROLLER_BUTTON_DPAD_UP:
            if(audio_volume < MIX_MAX_VOLUME) {
              audio_volume += 2;
            }
            else {
              audio_volume = MIX_MAX_VOLUME;
            }
            Mix_VolumeMusic(audio_volume);
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}

int main(int argc, char *argv[])
{
  Init();

  //Open controller
  int controllerport = 0;
  std::string controllername = "";

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

  while (true) {
    int numFiles = showFilePicker(window);
    pos = {45, 40, 500, 20};
    for (int i = 0; i < numFiles; i++) {
      SDL_FillRect(SDL_GetWindowSurface(window), &pos, SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0, 0, 0));
      pos.y += 20;
    }
    SDL_UpdateWindowSurface(window);
    PlayFile();
    if (controller != NULL) {
      formatString << "Opened controller " << controllername << " on port " << controllerport;
      PutToWindow(formatString.str(), Roboto);
    }
    formatString << "Now playing: " << fileToPlay;
    PutToWindow(formatString.str(), Roboto);
    while (Mix_PlayingMusic() == 1) {

     #if defined(NXDK)
      XVideoWaitForVBlank();
      #endif
      ProcessInput();
    }
    Mix_FreeMusic(music);
  }
  return 0;
}
