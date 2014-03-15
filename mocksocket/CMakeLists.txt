CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)
PROJECT(usegmock)

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -O2")

INCLUDE_DIRECTORIES($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include ./include)
LINK_DIRECTORIES($ENV{GMOCK_HOME}/lib)

SET(sources ./src/main.cpp ./src/common.cpp)

ADD_EXECUTABLE(test ${sources})

FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(test gmock gmock_main ${CMAKE_THREAD_LIBS_INIT})
