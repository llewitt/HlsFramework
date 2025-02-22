compile_unit_test(KERNEL wideBurstLoader
                  DEPENDS UnitTesting)

compile_xo(KERNEL wideBurstLoader
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/wideBurstLoader.cxx)

link_xclbins(PIPELINE BurstTransfer
             DEPENDS wideBurstLoader
                     wideFreeRunningKernel
                     wideUnloader)

