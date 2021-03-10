ifeq ($(USE_SOFTWARE_SDL_STANDARD),1)
SRCS += implementations/software/SDL/standard/system_software_SDL.cpp
SRCS += implementations/software/SDL/standard/player_software_SDL_mixer.cpp
SRCS += implementations/software/SDL/standard/sourcePicker_software_SDL_FilesystemX.cpp
CXXFLAGS += -DUSE_SOFTWARE_SDL_STANDARD=1
LDFLAGS += -lSDL2 -I/usr/include/SDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2
endif
SRCS += main.cpp
CXXFLAGS += -Wall -pedantic -g -Og
LDFLAGS += -lstdc++fs -std=c++17

all:
	$(CXX) $(SRCS) $(CXXFLAGS) -o XMusic $(LDFLAGS)
	mkdir build
	cp XMusic build/
	cp resources/Roboto-Regular.ttf build/
	cp resources/arrow.png build/
	cp resources/border.png build/
	mkdir build/Music
