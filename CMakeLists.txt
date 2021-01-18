cmake_minimum_required(VERSION 3.1)
project(XMusic)
set(CMAKE_CXX_STANDARD 17)
add_executable(XMusic player.cpp)
target_link_libraries(XMusic -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image)
target_include_directories(XMusic PUBLIC /usr/include/SDL2)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()