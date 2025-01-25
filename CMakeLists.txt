cmake_minimum_required(VERSION 3.30)
project(photomask C)

find_package(SDL2 REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

add_executable(photomask main.c)
target_include_directories(photomask PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(photomask PUBLIC ${SDL2_LIBRARIES})
target_compile_options(photomask PRIVATE -Wconversion -Wall -Wextra -Wdouble-promotion)