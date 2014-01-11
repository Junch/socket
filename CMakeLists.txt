CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)
PROJECT(Test C CXX)

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# Add gtest
EXTERNALPROJECT_ADD(
  googletest
  # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
  URL ${PROJECT_SOURCE_DIR}/ext/gtest-1.7.0.zip
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
EXTERNALPROJECT_GET_PROPERTY(googletest source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)

# This link_directories needs to be BEFORE the add_executable
EXTERNALPROJECT_GET_PROPERTY(googletest binary_dir)
LINK_DIRECTORIES(${binary_dir})

# Add test executable target
ADD_EXECUTABLE(MainTest ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Create dependency of MainTest on googletest
ADD_DEPENDENCIES(MainTest googletest)

# Specify MainTest's link libraries
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(MainTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# Configure Ctest
ENABLE_TESTING()
ADD_TEST(NAME MainTest COMMAND MainTest)
