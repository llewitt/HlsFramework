compile_unit_test(KERNEL extendingADataflow_SolutionA
                  DEPENDS UnitTesting)

compile_xo(KERNEL extendingADataflow_SolutionA
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/extendingADataflow_SolutionA.cxx)

link_xclbins(PIPELINE ExtendingADataflow_SolutionA
             DEPENDS loader
                     extendingADataflow_SolutionA
                     unloader)

