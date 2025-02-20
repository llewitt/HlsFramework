compile_unit_test(KERNEL wideLoader
                  DEPENDS UnitTesting)

compile_xo(KERNEL wideLoader
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/wideLoader.cxx)

compile_unit_test(KERNEL wideFreeRunningKernel
                  DEPENDS UnitTesting)

compile_xo(KERNEL wideFreeRunningKernel
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/wideFreeRunningKernel.cxx)

compile_unit_test(KERNEL wideUnloader
                  DEPENDS UnitTesting)

compile_xo(KERNEL wideUnloader
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/wideUnloader.cxx)

link_xclbins(PIPELINE WideStream
             DEPENDS wideLoader
                     wideFreeRunningKernel
                     wideUnloader)

