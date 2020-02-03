
# https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187

# Define our host system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)# Define the cross compiler locations

# using https://github.com/raspberrypi/tools
#SET(CMAKE_C_COMPILER   /net/pi/rpi-tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /net/pi/rpi-tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)

#using 'sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf' on Debian
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-gcc)

#SET(CMAKE_C_COMPILER   /net/pi/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /net/pi/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)


SET(CMAKE_FIND_ROOT_PATH /net/pi/rootfs-og/) # copied from working RPi0
#SET(CMAKE_SYSROOT /net/pi/rootfs-og/)

#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
#SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

# Search for programs only in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers only in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CFLAGS "${CFLAGS}" -m64)
SET(CXXFLAGS "${CXXFLAGS}" -m64)

#add_definitions(-Wall -std=c11)

#unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
#unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)


# cmake -D CMAKE_BUILD_TYPE=Debug -D CMAKE_TOOLCHAIN_FILE=../cmake/toolchain-rpi.cmake ..

