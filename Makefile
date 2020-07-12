all:
	gcc player.c -o Binaries/XMusic -lSDL2 -I/usr/include/SDL2 -lSDL2_mixer -I/usr/include/SDL2 -std=c99 -Wall -pedantic -g -Og
