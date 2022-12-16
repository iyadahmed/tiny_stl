add_library(tiny_stl "tiny_stl.cpp" "../include/tiny_stl/tiny_stl.hpp")
target_include_directories(tiny_stl PUBLIC "../include")
target_link_libraries(tiny_stl PRIVATE fast_float)