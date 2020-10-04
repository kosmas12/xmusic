all:
	g++-8 player.cpp -o XMusic -lSDL2 -I/usr/include/SDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -std=c++17 -Wall -pedantic -g -Og
