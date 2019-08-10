cmake_minimum_required(VERSION 3.0)

project(concentric-clock)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(external/rpi_ws281x-1.1.0)
include_directories(external/rpi_ws281x-1.1.0)


file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.c*)

include_directories(include)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} rpi_ws281x)
