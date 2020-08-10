all:
	gcc player.c -o XMusic -lSDL2 -I/usr/include/SDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -std=c99 -Wall -pedantic -g -Og
