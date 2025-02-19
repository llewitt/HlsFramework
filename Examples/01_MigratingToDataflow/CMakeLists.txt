compile_unit_test(KERNEL migratingToDataflow)
compile_xo(KERNEL migratingToDataflow
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/migratingToDataflow.cxx)

link_xclbins(PIPELINE MigratingToDataflow
             DEPENDS migratingToDataflow)

