compile_unit_test(KERNEL parameterisingFirmware_Scenario
                  DEPENDS UnitTesting)

compile_xo(KERNEL parameterisingFirmware_Scenario
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parameterisingFirmware_Scenario.cxx)

link_xclbins(PIPELINE ParameterisingFirmware_Scenario
             DEPENDS loader
                     parameterisingFirmware_Scenario
                     unloader)

