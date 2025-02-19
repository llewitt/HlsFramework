add_executable(host.exe src/host.cxx)
target_link_libraries(host.exe PRIVATE LibXRT)
target_link_libraries(host.exe PRIVATE nlohmann_json)

