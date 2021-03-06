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

#ifdef USE_SOFTWARE_SDL_STANDARD
#include "implementations/software/SDL/standard/player_software_SDL_mixer.h"
#include "implementations/software/SDL/standard/system_software_SDL.h"
#include "implementations/software/SDL/standard/sourcePicker_software_SDL_FilesystemX.h"
#endif

#ifdef USE_HARDWARE_SDL_PSP
#include "implementations/PSP/system_PSP.h"
#include "implementations/PSP/player_PSP_pspaudio.h"
#include "implementations/PSP/sourcePicker_PSP_sceio.h"
#endif

int main(int argc, char *argv[]) {

    Init(); // Initialize all required libraries and variables

    while (!exitted) { // While we haven't been asked to exit
        while (isPlaying()) { // If music is playing, this returns true, else false (can change as part of a backend hack if needed)
            ProcessInput(); // Process all the input the user gives us for the player controls
        }
        if (shouldStop) { // If the music shouldn't play again
            ClearScreen(); // Clear the screen
            FreeMusic(); // Get rid of the current music
            selectSource(); // Select source to play new music
        }
        PlaySource(); // Play the selected source
        UpdateScreen(); // Update the screen with the text from PlaySource()
    }
    return 0;
}
