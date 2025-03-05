compile_unit_test(KERNEL extendingADataflow_Scenario
                  DEPENDS UnitTesting)

compile_xo(KERNEL extendingADataflow_Scenario
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/extendingADataflow_Scenario.cxx)

link_xclbins(PIPELINE ExtendingADataflow_Scenario
             DEPENDS loader
                     extendingADataflow_Scenario
                     unloader)

