CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)
PROJECT(Test)

# Create main.cpp which uses gtest
FILE(WRITE src/main.cpp "#include \"gtest/gtest.h\"\n\n")
FILE(APPEND src/main.cpp "TEST(A, B) { SUCCEED(); }\n")
FILE(APPEND src/main.cpp "int main(int argc, char **argv) {\n")
FILE(APPEND src/main.cpp "  testing::InitGoogleTest(&argc, argv);\n")
FILE(APPEND src/main.cpp "  return RUN_ALL_TESTS();\n")
FILE(APPEND src/main.cpp "}\n")

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# Add gtest
ExternalProject_Add(
  googletest
  # TODO: find the folder rather than the tar.gz
  URL ~/Softwares/gtest-svn.tar.gz
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
ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)

# Add test executable target
ADD_EXECUTABLE(MainTest ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Create dependency of MainTest on googletest
ADD_DEPENDENCIES(MainTest googletest)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
#TARGET_LINK_LIBRARIES(MainTest ${binary_dir}/DebugLibs/libgtest.a)

#include_directories($ENV{GTEST_ROOT}/include)
link_directories(${binary_dir})
#TARGET_LINK_LIBRARIES(MainTest gtest gtest_main)
target_link_libraries(MainTest ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(MainTest ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
