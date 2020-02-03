cmake_minimum_required(VERSION 3.0)

project(concentric-clock)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(external/rpi_ws281x-1.1.0)
include_directories(external/rpi_ws281x-1.1.0)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.c*)

include_directories(include)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})


# the "wrong" way

#target_link_libraries(${PROJECT_NAME} rpi_ws281x wiringPi pthread) 

#find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
#target_link_libraries(${PROJECT_NAME} rpi_ws281x ${WIRINGPI_LIBRARIES} pthread) # wiringPi


# the "right" way

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Locate libraries and headers
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED) 

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS}) 

# Link against libraries
target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} rpi_ws281x) 
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT}) 

