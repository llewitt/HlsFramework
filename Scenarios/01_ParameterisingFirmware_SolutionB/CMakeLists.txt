compile_unit_test(KERNEL parameterisingFirmware_SolutionB
                  DEPENDS UnitTesting)

compile_xo(KERNEL parameterisingFirmware_SolutionB
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parameterisingFirmware_SolutionB.cxx)

link_xclbins(PIPELINE ParameterisingFirmware_SolutionB
             DEPENDS loader
                     parameterisingFirmware_SolutionB
                     unloader)

