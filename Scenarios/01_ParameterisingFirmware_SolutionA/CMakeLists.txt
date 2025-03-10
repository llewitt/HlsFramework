compile_unit_test(KERNEL parameterisingFirmware_SolutionA
                  DEPENDS UnitTesting)

compile_xo(KERNEL parameterisingFirmware_SolutionA
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parameterisingFirmware_SolutionA.cxx)

link_xclbins(PIPELINE ParameterisingFirmware_SolutionA
             DEPENDS loader
                     parameterisingFirmware_SolutionA
                     unloader)

