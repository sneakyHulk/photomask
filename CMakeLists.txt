cmake_minimum_required(VERSION 3.30)
project(photomask C)

find_package(SDL2 REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(photomask main.c)
target_include_directories(photomask PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(photomask PUBLIC ${SDL2_LIBRARIES})
