compile_unit_test(KERNEL splitter_SolutionA
                  DEPENDS UnitTesting)

compile_xo(KERNEL splitter_SolutionA
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/splitter_SolutionA.cxx)

link_xclbins(PIPELINE Splitter_SolutionA
             DEPENDS loader
                     splitter_SolutionA
                     unloader)

