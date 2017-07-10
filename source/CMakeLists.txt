cmake_minimum_required(VERSION 3.0)
Project(BufferedSorting)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-std=gnu99)

file(GLOB_RECURSE SOURCES *.c *.cpp *.h)

add_executable(BufferedSorting ${SOURCES})
