# xmusic
XMusic, a music player for Xbox and PC using SDL2

**NOTE: This has been moved over from the My-code-stuff repository**

**This program is licensed under the GNU General Public License, Version 2. Read COPYING for details.**

**The Makefiles for Windows suppose that you are running some sort of UNIX subsystem (e.g. WSL, CYGWIN) and have set up MinGW. If you are using Windows and want to compile from source, make sure to change the library paths so that they reflect your real paths**

How to use:

XMusic has a file selector built-in, from which you can select your file. In order for it to work, you need to put your music files inside a folder called Music, either on your D: drive (for Xbox and Windows) or in a Music subdirectory of where you are running XMusic from. Lastly, you can change the ROOT in the beginning of sdlFilePicker.hpp to anything you want if you don't want to do this.  This file browser works both on Xbox and PC. Select a file using either the arrow keys on your keyboard or the Dpad buttons on your controller. For now, it can properly display 20 files

# Supported file types

XMusic supports all the file formats that SDL\_mixer can play. This means .wav, .ogg, MIDI files, MOD files, .flac, .mp3 and OPUS files.

On Original Xbox, the only playable formats are .wav and .ogg, due to limitations in the SDL\_mixer port. This is expected to improve in the future.