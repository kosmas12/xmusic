cmake_minimum_required(VERSION 3.16)
project(XMusic)
set(CMAKE_CXX_STANDARD 17)

option(USE_SOFTWARE_SDL_STANDARD "Use the standard (PC/Xbox) software SDL backend" ON)

if(USE_SOFTWARE_SDL_STANDARD)
    add_definitions(-DUSE_SOFTWARE_SDL_STANDARD)
    file(GLOB SOURCES_FILES_LIBS "implementations/software/SDL/standard/*.cpp")
    set(SOURCES_FILES main.cpp)
    add_executable(XMusic ${SOURCES_FILES_LIBS} ${SOURCES_FILES})
    target_link_libraries(XMusic -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image)
    target_include_directories(XMusic PUBLIC /usr/include/SDL2)
    configure_file(resources/arrow.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    configure_file(resources/border.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    configure_file(resources/Roboto-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Music)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nxdk.wav ${CMAKE_CURRENT_BINARY_DIR}/Music COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Pause\ Test.wav ${CMAKE_CURRENT_BINARY_DIR}/Music COPYONLY)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()