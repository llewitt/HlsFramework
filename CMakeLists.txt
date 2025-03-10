cmake_minimum_required(VERSION 3.15...3.31)
project(HlsFramework
        VERSION 1.0
        LANGUAGES CXX)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Targets)

function(compile_unit_test)
  set(options)
  set(oneValueArgs KERNEL)
  set(multiValueArgs SOURCES DEPENDS)
  cmake_parse_arguments(compile_unit_test
                        "${options}" 
                        "${oneValueArgs}"
                        "${multiValueArgs}" 
                        ${ARGN})

  add_executable(${compile_unit_test_KERNEL}UnitTest.exe 
                 src/${compile_unit_test_KERNEL}.cxx 
                 ${compile_unit_test_SOURCES})

  target_link_libraries(${compile_unit_test_KERNEL}UnitTest.exe PRIVATE UnitTesting)

  target_include_directories(${compile_unit_test_KERNEL}UnitTest.exe PRIVATE include)
  target_include_directories(${compile_unit_test_KERNEL}UnitTest.exe PRIVATE /opt/Xilinx/Vitis_HLS/2024.1/include)

  add_test(NAME ${compile_unit_test_KERNEL}UnitTest COMMAND ${compile_unit_test_KERNEL}UnitTest.exe)
  add_custom_target(${compile_unit_test_KERNEL}UnitTestRun 
              COMMAND ${CMAKE_CTEST_COMMAND} 
              DEPENDS ${compile_unit_test_KERNEL}UnitTest.exe)
endfunction()

function(compile_xo)
  set(options)
  set(oneValueArgs KERNEL)
  set(multiValueArgs DEPENDS INCLUDES)
  cmake_parse_arguments(compile_xo
                        "${options}" 
                        "${oneValueArgs}"
                        "${multiValueArgs}" 
                        ${ARGN})


  list(TRANSFORM compile_xo_INCLUDES
       PREPEND "--include="
       OUTPUT_VARIABLE _compile_xo_INCLUDES)

  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}.xo
                     DEPENDS ${compile_xo_DEPENDS}
                     COMMAND v++ --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 
                                 --target hw_emu
                                 ${SAVE_TEMPS}
                                 --compile 
                                 --log_dir ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}_log
                                 --report_dir ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}_report
                                 --temp_dir ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}_temp
                                 --kernel ${compile_xo_KERNEL} 
                                 --output ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}.xo 
                                 ${_compile_xo_INCLUDES}
                                 ${CMAKE_CURRENT_SOURCE_DIR}/src/${compile_xo_KERNEL}.cxx
                     DEPENDS ${compile_xo_DEPENDS})

  unset(_compile_xo_INCLUDES)
  add_custom_target(${compile_xo_KERNEL}
                    ALL
                    DEPENDS ${CMAKE_BINARY_DIR}/Targets/${compile_xo_KERNEL}.xo)
endfunction()

function(link_xclbins)
  set(options)
  set(oneValueArgs PIPELINE)
  set(multiValueArgs DEPENDS)
  cmake_parse_arguments(link_xclbins
                        "${options}" 
                        "${oneValueArgs}"
                        "${multiValueArgs}" 
                        ${ARGN})

  if (${EMULATION_MODE_sw_emu})
    link_xclbin(PIPELINE ${link_xclbins_PIPELINE}
                         DEPENDS ${link_xclbins_DEPENDS}
                         EMULATION_MODE sw_emu)
  endif()

  if (${EMULATION_MODE_hw_emu})
    link_xclbin(PIPELINE ${link_xclbins_PIPELINE}
                         DEPENDS ${link_xclbins_DEPENDS}
                         EMULATION_MODE hw_emu)
  endif()

  if (${EMULATION_MODE_hw})
    link_xclbin(PIPELINE ${link_xclbins_PIPELINE}
                         DEPENDS ${link_xclbins_DEPENDS}
                         EMULATION_MODE hw)
  endif()
endfunction()

function(link_xclbin)
  set(options)
  set(oneValueArgs PIPELINE EMULATION_MODE)
  set(multiValueArgs DEPENDS)
  cmake_parse_arguments(link_xclbin
                        "${options}" 
                        "${oneValueArgs}"
                        "${multiValueArgs}" 
                        ${ARGN})

  list(TRANSFORM link_xclbin_DEPENDS
       PREPEND "${CMAKE_BINARY_DIR}/Targets/"
       OUTPUT_VARIABLE _link_xclbin_DEPENDS)

  list(TRANSFORM _link_xclbin_DEPENDS
       APPEND ".xo"
       OUTPUT_VARIABLE __link_xclbin_DEPENDS)

  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}.xclbin
                     COMMAND v++ --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 
                                 -g
                                 ${SAVE_TEMPS}
                                 --log_dir ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}_log
                                 --report_dir ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}_report
                                 --temp_dir ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}_temp
                                 --link
                                 --target ${link_xclbin_EMULATION_MODE}
                                 --kernel_frequency 400
                                 --config ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
                                 --output ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}.xclbin
                      DEPENDS ${link_xclbin_DEPENDS}
                              ${__link_xclbin_DEPENDS}
                              ${CMAKE_CURRENT_SOURCE_DIR}/config.ini)

  unset(_link_xclbin_DEPENDS)
  unset(__link_xclbin_DEPENDS)

  add_custom_target(${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}
                    ALL
                    DEPENDS ${CMAKE_BINARY_DIR}/Targets/${link_xclbin_PIPELINE}_${link_xclbin_EMULATION_MODE}.xclbin)
endfunction()

enable_testing()

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
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
add_subdirectory(Scenarios)
add_subdirectory(Host)

