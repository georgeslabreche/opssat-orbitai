CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#SET(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
#SET(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")

PROJECT(orbitai C CXX)

FIND_PACKAGE(Threads REQUIRED)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -static -O3 -std=c++14  -IMochiMochi -Ieigen")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_C_LINK_EXECUTABLE "-lboost_serialization -lboost_program_options")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")
ADD_EXECUTABLE(orbitai.out orbitai.cpp)
TARGET_LINK_LIBRARIES(orbitai.out ${CMAKE_C_LINK_EXECUTABLE})
