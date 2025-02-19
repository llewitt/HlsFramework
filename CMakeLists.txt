cmake_minimum_required(VERSION 3.15...3.31)
project(HlsFramework
        VERSION 1.0
        LANGUAGES CXX)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Targets)
include(CMakeFunctions.txt)

enable_testing()

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(nlohmann_json REQUIRED)

# FIXME this a hack as this library is currently not distributed in CVMFS
add_library(LibXRT SHARED IMPORTED)
set_target_properties(LibXRT PROPERTIES
  IMPORTED_LOCATION "$ENV{XILINX_XRT}/lib/libxrt_coreutil.so"
  INTERFACE_INCLUDE_DIRECTORIES "$ENV{XILINX_XRT}/include"
)

add_library(LibXOCL SHARED IMPORTED)
set_target_properties(LibXOCL PROPERTIES
  IMPORTED_LOCATION "$ENV{XILINX_XRT}/lib/libxilinxopencl.so"
  INTERFACE_INCLUDE_DIRECTORIES "$ENV{XILINX_XRT}/include"
)

if(${DO_SAVE_TEMPS})
  set(SAVE_TEMPS --save-temps)
endif()

add_subdirectory(UnitTesting)
add_subdirectory(Examples)
add_subdirectory(Host)

