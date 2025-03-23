package_xo(KERNEL incorporatingRtl
           INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src)

link_xclbins(PIPELINE IncorporatingRtl
             DEPENDS loader
                     incorporatingRtl
                     unloader)

