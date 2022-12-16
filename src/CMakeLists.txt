add_library(tiny_stl "tiny_stl.cpp" "../include/tiny_stl/tiny_stl.hpp")
target_include_directories(tiny_stl PUBLIC "../include")
target_link_libraries(tiny_stl PRIVATE fast_float)
target_compile_features(tiny_stl PUBLIC cxx_std_11) # Enable C++11 standard for std::make_unique