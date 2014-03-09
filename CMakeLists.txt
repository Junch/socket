CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)
PROJECT(Test)

# Required for gCov
OPTION(coverage "Built with the gCov/lcov support" OFF)

ADD_DEFINITIONS(
  -g
  -O0
  -Wall
  -std=c++11
)

MESSAGE(STATUS "The coverage %coverage%")

IF (coverage)
  MESSAGE(STATUS "The coverage %coverage%")
  ADD_DEFINITIONS(
    -fprofile-arcs    # Required for gCov
    -ftest-coverage   # Required for gCov
  )
ENDIF()

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# Add gtest
EXTERNALPROJECT_ADD(
  googlemock
  # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
  URL ${PROJECT_SOURCE_DIR}/ext/gmock-1.7.0.zip
  # Force separate output paths for debug and release builds to allow easy
  # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
  CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
  -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
  -Dgtest_force_shared_crt=OFF
  # Disable install step
  INSTALL_COMMAND ""
  # Wrap download, configure and build steps in a script to log output
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

# Specify include dir
EXTERNALPROJECT_GET_PROPERTY(googlemock source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)
INCLUDE_DIRECTORIES(${source_dir}/gtest/include)

# This link_directories needs to be BEFORE the add_executable
EXTERNALPROJECT_GET_PROPERTY(googlemock binary_dir)
LINK_DIRECTORIES(${binary_dir})

# Add test executable target
ADD_EXECUTABLE(MainTest ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Create dependency of MainTest on googlemock
ADD_DEPENDENCIES(MainTest googlemock)

ADD_EXECUTABLE(server ${PROJECT_SOURCE_DIR}/src/server.cpp)

ADD_EXECUTABLE(client ${PROJECT_SOURCE_DIR}/src/client.cpp)

# Required for gCov
IF (coverage)
  SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
ENDIF()

# Specify MainTest's link libraries
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(MainTest gmock gmock_main ${CMAKE_THREAD_LIBS_INIT})

# Configure Ctest
IF (coverage)
  ENABLE_TESTING()
  ADD_TEST(NAME MainTest COMMAND MainTest)

  # Code Coverage
  SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
  IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    INCLUDE(CodeCoverage)
    SETUP_TARGET_FOR_COVERAGE(${PROJECT_NAME}_coverage MainTest coverage)
  ENDIF()
ENDIF()
