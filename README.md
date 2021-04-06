# xmusic
XMusic, a music player for Xbox and PC using SDL2

**NOTE: This has been moved over from the My-code-stuff repository**

**This program is licensed under the GNU General Public License, Version 2. Read COPYING for details.**

**The Makefiles for Windows suppose that you have set up SDL2, SDL2\_mixer, SDL2\_image and SDL2\_ttf on MinGW's default search path. If you haven't done this, then you can use the -I and -L flags to specify the directories where your libraries and includes are located**

How to use:

XMusic has a file selector built-in, from which you 
can select your file. This file browser works both on 
Xbox and PC. Select a file using either the arrow keys 
on your keyboard or the Dpad buttons on your controller. 
For now, it can properly display 20 files.

# Supported file types

XMusic supports all the file formats that SDL\_mixer can play. This means .wav, .ogg, MIDI files, MOD files, .flac, .mp3 and OPUS files.

On Original Xbox, the only playable formats are .wav and .ogg, due to limitations in the SDL\_mixer port. This is expected to improve in the future.

# Controls
PC:

A on a controller or Space on a keyboard will toggle pause.

B on a controller or Escape on a keyboard will exit the program if in the file picker, will exit to file picker if playing a song.

Backspace will go up one directory. Also if you pick a directory in the file picker, Enter will enter that directory.

Y on a controller or R on a controller in the file picker will refresh the directory listing.

Xbox:

A will toggle pause.

B will exit to dashboard if the program is in the file picker, will exit to file picker if playing a song.

Y in the file picker will refresh the directory listing.

PSP:

D-Pad Up/Down to change files in file picker.

X in file picker to confirm selection, use it for loop on/off in the player.

O in player for pause/resume.

Triangle to reset to beginning of file.

Square to exit back to file picker.

# How to compile

For Linux, Xbox (nxdk) and Windows (32 bit and 64 bit), there are available Makefiles which allow you to easily compile XMusic. Just make sure to have the required dependencies (SDL2, SDL2_mixer, SDL2_ttf and SDL2_image) installed on your compiler's default search path.

For UNIX systems:

`make USE_SOFTWARE_SDL_STANDARD=1` (requires G++ 8 or higher, or equivalent clang)

For Xbox:

`make -f Makefile.nxdk USE_SOFTWARE_SDL_STANDARD=1` (Also requires setting the correct NXDK_DIR if it isn't there already)

For Windows:

`make -f MakefileWin32 USE_SOFTWARE_SDL_STANDARD=1` for 32 bit

`make -f MakefileWin64 USE_SOFTWARE_SDL_STANDARD=1` for 64 bit

With CMake:

```
mkdir build 
cd build
cmake .. -DUSE_SOFTWARE_SDL_STANDARD=ON
make
```

For PSP:

`make -f Makefile.psp` (NOTE: Supports only .mp3 music for now)

Windows also requires that you pack all the DLL files in the same directory as XMusic.exe

# Implementations

XMusic works using an implementation of a player and a file picker. However, some people
might prefer using one backend to play audio than another, for example, someone might
prefer using PortAudio to play sound rather than XMusic's default SDL_mixer. Or someone
would want to use their graphics card for drawing the interface rather than their processor for any reason.
This is why XMusic supports using custom implementations. Someone with enough programmming
knowledge can, for example, make their own XMusic implementation using the backend that
they want. This is a list of currently supported implementations:

| Name                                  | Implementing                       | Made for         |
|---------------------------------------|------------------------------------|------------------|
| player_software_SDL_mixer             | player (software, SDL_mixer)       | PC, Xbox         |
| sourcePicker_software_SDL_FilesystemX | source picker (software, SDL)      | PC, Xbox         |
| system_software_SDL                   | global needed code (software, SDL) | PC, Xbox         |

**Standard implementations support PC and Original Xbox (nxdk toolchain) and all the other
implementations have their supported platform(s) on their name**

# Notes

**On Xbox, the RECOMMENDED file format is OGG/Vorbis, preferably created
with FFMpeg and has a 48000Hz sample rate. Anything lower may cause 
a popping sound while playing!**