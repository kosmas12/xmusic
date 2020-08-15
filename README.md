# xmusic
XMusic, a music player for Xbox and PC using SDL2

**NOTE: This has been moved over from the My-code-stuff repository**

**This program is licensed under the GNU General Public License, Version 2. Read COPYING for details.**

**The Makefiles for Windows suppose that you are running some sort of UNIX subsystem (e.g. WSL, CYGWIN) and have set up MinGW. If you are using Windows and want to compile from source, make sure to change the library paths so that they reflect your real paths**

How to use:

XMusic (as of its current state) uses file paths to open your files. For example, say that on a *nix system you have the bundled file Pause Test.wav on your Desktop. Its path will be:
```
/home/username/Desktop/Pause Test.wav
```

Or if you are on Windows, XMusic will work with this:
```
C:\\Users\\username\\Desktop\\Pause Test.wav
```

After you type in the path and press Enter, your file will play. The only way to pause files right now is by holding A on a controller that was connected before running XMusic. Keyboard support will be added in the future.

On Xbox, you will be presented with a file selector to choose your file from