cmake_minimum_required(VERSION 3.15...3.31)
project(HlsFramework
        VERSION 1.0
        LANGUAGES CXX)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Targets)
include(CMakeFunctions.txt)

enable_testing()

find_package(OpenCL)
find_package(XRT)
find_package(Boost COMPONENTS unit_test_framework)

if(${DO_SAVE_TEMPS})
  set(SAVE_TEMPS --save-temps)
endif()

add_subdirectory(UnitTesting)
add_subdirectory(Examples)

