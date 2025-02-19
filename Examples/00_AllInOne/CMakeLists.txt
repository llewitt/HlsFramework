compile_unit_test(KERNEL allInOne)
compile_xo(KERNEL allInOne
           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/allInOne.cxx)

link_xclbins(PIPELINE AllInOne
             DEPENDS allInOne)

