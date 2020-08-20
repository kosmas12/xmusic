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
#include <stdio.h>
#include <sstream>
#include <stdbool.h>

//static Uint8 *audio_position = NULL; 
//static Uint32 audio_length = 0;
//Uint8 *wavBuffer = NULL;
static int audio_open = 0;
static Mix_Music *music = NULL;
std::stringstream formatString;
std::string formatStringstr;


SDL_AudioDeviceID deviceID = 0;
SDL_Window* window = NULL;

/*
void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_length == 0)
		return;
	
	len = (len > audio_length ? audio_length : len);
	SDL_memcpy(stream, audio_position, len); // simply copy from one buffer into the other
	
	audio_position += len;
	audio_length -= len;
} */

static void Quit(Mix_Music *music, int exitcode) {
  Mix_FreeMusic(music);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_free(controller);
  IMG_Quit();
  TTF_CloseFont(Roboto);
  TTF_Quit();
	//SDL_FreeWAV(wavBuffer);
	SDL_Quit();
  #if defined (NXDK)
  XReboot();
  #endif
  exit(exitcode);
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
    int audio_volume = MIX_MAX_VOLUME;
    int looping = 0;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
      formatString << "Couldn't open audio: " << SDL_GetError();
      formatStringstr = formatString.str();
      text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
      SDL_BlitSurface(text, NULL, windowSurface, &pos);
      SDL_UpdateWindowSurface(window);
      pos.y += 15;
      formatString.str("");
    } 
    else {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      formatString << "Opened audio at " << audio_rate << " Hz " << (audio_format&0xFF) <<  " bit " << (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : "") 
      <<  ((audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo " : "mono ") <<  audio_buffers << " bytes audio buffer";
      formatStringstr = formatString.str();
      text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
      SDL_BlitSurface(text, NULL, windowSurface, &pos);
      SDL_UpdateWindowSurface(window);
      pos.y += 15;
      formatString.str("");
    }
    audio_open = 1;

    text = TTF_RenderText_Blended(Roboto, "Setting volume", color);
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    formatString.str("");
    SDL_UpdateWindowSurface(window);
    pos.y += 15;
    Mix_VolumeMusic(audio_volume);
    formatString << "Opening " << fileToPlay;
    formatStringstr = formatString.str();
    text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    SDL_UpdateWindowSurface(window);
    pos.y += 15;
    formatString.str("");
    SDL_RWops *rw = SDL_RWFromFile(fileToPlay.c_str(), "rb");

  if (rw == NULL) {
    formatString << "Couldn't open " << fileToPlay << ": " << Mix_GetError();
    formatStringstr = formatString.str();
    text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    SDL_UpdateWindowSurface(window);
    pos.y += 15;
    formatString.str("");
    Quit(music, 2);
    free(rw);
  }
  formatString << "Loading " << fileToPlay;
  formatStringstr = formatString.str();
  text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
  SDL_BlitSurface(text, NULL, windowSurface, &pos);
  SDL_UpdateWindowSurface(window);
  pos.y += 15;
  formatString.str("");
  music = Mix_LoadMUS_RW(rw, SDL_TRUE);
  if (music == NULL) {
    formatString << "Couldn't load " << fileToPlay << ": " << Mix_GetError();
    formatStringstr = formatString.str();
    text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    SDL_UpdateWindowSurface(window);
    pos.y += 15;
    formatString.str("");
    Quit(music, 3);
    free(rw);
  }
  formatString << "Loaded " << fileToPlay;
  formatStringstr = formatString.str();
  text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
  SDL_BlitSurface(text, NULL, windowSurface, &pos);
  SDL_UpdateWindowSurface(window);
  pos.y += 15;
  formatString.str("");
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

#ifndef NXDK
  SDL_Init(SDL_INIT_EVERYTHING);
#else
  SDL_Init(SDL_INIT_GAMECONTROLLER|SDL_INIT_VIDEO|SDL_INIT_AUDIO);
#endif
  int mixflags = MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MID|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OPUS;
  int mixinitted = Mix_Init(mixflags);
  printf("Return value of Mix_Init(): %d\n", mixinitted);
  window = SDL_CreateWindow( "XMusic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
  int ret = InitFilePicker();
  if(!ret) {
      printf("File Picker disabled as it was not possible to init SDL_Image and/or SDL_TTF %u",ret);
  }
  SDL_Delay(1500);


}

int main(int argc, char *argv[])
{
  Init();

  //Open controller
  int controllerport = 0;
  const char* controllername = NULL;

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
      SDL_FillRect(windowSurface, &pos, SDL_MapRGB(windowSurface->format, 0, 0, 0));
      pos.y += 20;
    }
    SDL_UpdateWindowSurface(window);
    PlayFile();
    if (controller != NULL)
    {
      formatString << "Opened controller " << controllername << " on port" << controllerport;
      formatStringstr = formatString.str();
      text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
      SDL_BlitSurface(text, NULL, windowSurface, &pos);
      SDL_UpdateWindowSurface(window);
      pos.y += 15;
    }
    formatString.str("");
    formatString << "Now playing: " << fileToPlay;
    formatStringstr = formatString.str();
    text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
    SDL_BlitSurface(text, NULL, windowSurface, &pos);
    SDL_UpdateWindowSurface(window);
    pos.y += 15;
    formatString.str("");  
    while (Mix_PlayingMusic() == 1) {

      #if defined (NXDK)
      XVideoWaitForVBlank();

      debugClearScreen();

      formatString << "Now playing: " << fileToPlay << std::endl;
      formatStringstr = formatString.str();
      text = TTF_RenderText_Blended(Roboto, formatStringstr.c_str(), color);
      SDL_BlitSurface(text, NULL, windowSurface, &pos);
      SDL_UpdateWindowSurface(window);
      pos.y += 15;
      formatString.str("");
      #endif

      #if !defined (NXDK)
      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
        {
          Quit(music, 0);
        }
      }
      #endif
    
      if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
        //SDL_PauseAudioDevice(deviceID, 1);
        Mix_PauseMusic();
      }
    
      else
      {
        //SDL_PauseAudioDevice(deviceID, 0); //0 means unpaused
        Mix_ResumeMusic();
      }

      if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
      {
        break;
      }
      
    }
    
  }
  Quit(music, 0);
  return 0;
}
