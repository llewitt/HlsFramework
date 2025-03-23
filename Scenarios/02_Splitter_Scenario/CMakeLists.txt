compile_unit_test(KERNEL splitter_Scenario
                  DEPENDS UnitTesting)

compile_xo(KERNEL splitter_Scenario
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/splitter_Scenario.cxx)

link_xclbins(PIPELINE Splitter_Scenario
             DEPENDS loader
                     splitter_Scenario
                     unloader)

