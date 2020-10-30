all:
	g++-8 player.cpp -o XMusic -lstdc++fs -lSDL2 -I/usr/include/SDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -std=c++17 -Wall -pedantic -g -Og
	mkdir build
	cp XMusic build/
	cp Roboto-Regular.ttf build/
	cp arrow.png build/
	cp border.png build/
	mkdir build/Music
