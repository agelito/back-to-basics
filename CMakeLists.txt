cmake_minimum_required(VERSION 3.24)
project(back_to_basics)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_STANDARD 11)

add_executable(hello_world src/hello_world.c)
