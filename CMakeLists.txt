cmake_minimum_required (VERSION 3.10)

project(kmeansUE1 VERSION 1.0 LANGUAGES CXX)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  string(CONCAT errormsg
    "You cannot build in a source directory (or any directory with "
    "a CMakeLists.txt file). Please make a build subdirectory. Feel "
    "free to remove CMakeCache.txt and CMakeFiles.")
  message(FATAL_ERROR "${errormsg}")
endif()

# Require C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

# External config
# ---------------
# CTest
include(CTest)
# ---------------
# GoogleTest
# Prevent overriding the parent project's
# compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# ---------------
# OpenCV
find_package(OpenCV REQUIRED)
# UNCOMMENT CONDITIONAL IF CMAKE < 4.0
if(OpenCV_FOUND)
  message(STATUS "Indexing OpenCV headers")
  include_directories(${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

# Internal sub-indexing
add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(test)

# External sub-indexing
add_subdirectory(external/googletest)
