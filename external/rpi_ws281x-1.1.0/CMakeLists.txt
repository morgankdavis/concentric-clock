
cmake_minimum_required(VERSION 3.0)

project(rpi_ws281x)

file(GLOB HEADERS *.h)
file(GLOB SOURCES *.c*)

include_directories(rpi_ws281x)

add_library(rpi_ws281x STATIC ${HEADERS} ${SOURCES})

