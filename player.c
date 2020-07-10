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
#include <stdio.h>
#define NUMFILES 50 // Define file limit to use in the File Browser function GetFiles()
#include <stdbool.h>

//static Uint8 *audio_position = NULL; 
//static Uint32 audio_length = 0;
//Uint8 *wavBuffer = NULL;
static int audio_open = 0;
static Mix_Music *music = NULL;


char fileToPlay[210];
SDL_AudioDeviceID deviceID = 0;
SDL_GameController *controller = NULL;

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
  free(controller);
	//SDL_FreeWAV(wavBuffer);
	SDL_Quit();
  #if defined (NXDK)
  XReboot();
  #endif
  exit(exitcode);
} 

static void PlayFile() {

  /*
  //Uint32 wavLength;
  //SDL_AudioSpec wavSpec;

  SDL_LoadWAV(fileToPlay, &wavSpec, &wavBuffer, &wavLength);//FIXME: Ask for file at runtime
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
      printf("Couldn't open audio: %s\n", SDL_GetError());
    } 
    else {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      printf("Opened audio at %d Hz %d bit%s %s %d bytes audio buffer\n", audio_rate,
      (audio_format&0xFF), (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
      (audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono",
      audio_buffers);
    }
    audio_open = 1;

    printf("Setting volume\n");
    Mix_VolumeMusic(audio_volume);
    printf("Opening %s\n", fileToPlay);

#if 1
    SDL_RWops *rw = SDL_RWFromFile(fileToPlay, "rb");
#else
    FILE* f = fopen(songs[i], "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buff = malloc(size);
    fread(buff, 1, size, f);
    fclose(f);
    SDL_RWops *rw = SDL_RWFromMem(buff, size);
#endif

  if (rw == NULL) {
    printf("Couldn't open %s: %s\n",
    fileToPlay, Mix_GetError());
    Quit(music, 2);
    free(rw);
  }
  printf("Loading %s\n", fileToPlay);
  music = Mix_LoadMUS_RW(rw, SDL_TRUE);
  if (music == NULL) {
    printf("Couldn't load %s: %s\n",
    fileToPlay, Mix_GetError());
    Quit(music, 3);
    free(rw);
  }
  printf("Loaded %s\n", fileToPlay);
  Mix_PlayMusic(music, looping);
}

static void Init() {

  #if defined (NXDK)
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  debugClearScreen();
  #endif

  SDL_Init(SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);
  int mixflags = MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MID|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OPUS;
  int mixinitted = Mix_Init(mixflags);

  printf("Return value of Mix_Init(): %d\n", mixinitted);
  SDL_Delay(1500);


}

#if defined (NXDK)
typedef struct
{
  int fileIndex;
  char fileName[50];
  char filePath[150];
}file;

file[] GetFiles(char* driveletter) {
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  file foundFiles[NUMFILES] = {NULL};

  size_t currentFileDirCount = 0;

  char* driveWav;
  sprintf(driveWav, "%s\\.wav", driveLetter);

  hFind = FindFirstFileA(driveWav, &findFileData);

  do {
    XVideoWaitForVBlank();
    debugClearScreen();
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    } 
    else {
      if (currentFileDirCount < NUMFILES) {
        foundFiles[currentFileDirCount].fileIndex = currentFileDirCount;
        strcpy(foundFiles[currentFileDirCount].fileName, findFileData.cFileName);
        sprintf(foundFiles[currentFileDirCount].filePath, "%s\\%s", driveletter, foundFiles[currentFileDirCount].fileName);
        currentFileDirCount++;
      }
    }
  } 
  while (FindNextFileA(hFind, &findFileData) != 0);
  FindClose(hFind);

  return foundFiles;
}

void listFiles(const file files[]) {
  for (int i = 0; i < NUMFILES) {
    if (files[i] != NULL) {
      printf("%d ", files[i].fileIndex);
      printf("%s\n", files[i].filePath);
    }
    else {
      return;
    }
  }
}


static int FileBrowser() {

  files[50] = GetFiles("D:");

  int currentIndex = 0;

  while (true)
  {
    XVideoWaitForVBlank();
    SDL_GameControllerUpdate();
    debugClearScreen();
    
    listFiles(files);

    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
      if (currentIndex != 0) {
        currentIndex--;
      } 
      else {
        currentIndex = 0;
      }
    }
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
      if (currentIndex != currentFileDirCount - 1) {
        currentIndex++;
      } 
      else {
        currentIndex = currentFileDirCount - 1;
      }
    }
    

    printf("\nYour current selected file is: %s (Index number %d)\n", files[currentIndex].fileName, currentIndex);
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
      strcpy(fileToPlay, files[currentIndex].filePath);
      free(files);
      break;
    }
  }

  return 0;
}

#endif

int main()
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

  #if defined (NXDK)
  if (FileBrowser() == 1)
  {
    printf("Error! Halting execution!");
    Sleep(2500);
    Quit(music, 5);
  }
  #else
  printf("Tell me the path to the file that you want to play: ");
  
  int numargs = scanf("%[^\t\n]", fileToPlay);
  if (numargs == 0)
  {
    printf("No file entered. Exiting...\n");
    Quit(music, 4);
  }
  
  #endif

  PlayFile();

  #if !defined (NXDK)
  printf("Opened controller: %s on port %d\n", controllername, controllerport);
  printf("Now playing: %s\n", fileToPlay);
  #endif

  while (Mix_PlayingMusic() == 1) {   
    #if defined (NXDK)
    XVideoWaitForVBlank();

    debugClearScreen();

    printf("Opened controller: %s on port %d\n", controllername, controllerport);
    printf("Now playing: %s\n", fileToPlay);
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

  Quit(music, 0);
  return 0;
}
