compile_unit_test(KERNEL loader
                  DEPENDS UnitTesting)
compile_xo(KERNEL loader
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/loader.cxx)

compile_unit_test(KERNEL freeRunningKernel
                  DEPENDS UnitTesting)

compile_xo(KERNEL freeRunningKernel
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/freeRunningKernel.cxx)

compile_unit_test(KERNEL unloader
                  DEPENDS UnitTesting)

compile_xo(KERNEL unloader
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/unloader.cxx)

#link_xclbins(PIPELINE MigratingToFreeRunningKernels
#             DEPENDS loader
#                     freeRunningKernel
#                     unloader)

