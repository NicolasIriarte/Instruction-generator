# A very simple example of a configured file that might need to be
# converted to one that is publicly installed in the case that
# you are developing a library
configure_file("Config.hpp.in" "${CMAKE_BINARY_DIR}/configured_files/include/Nemu/CMake/Config.hpp" ESCAPE_QUOTES)
