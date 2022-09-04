cmake_minimum_required(VERSION 3.24)
project(back_to_basics)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(back_to_basics ${SDL2_INCLUDE_DIRS})

add_executable(back_to_basics src/main.c src/game_window.c src/renderer.c)
target_link_libraries(back_to_basics ${SDL2_LIBRARIES})
