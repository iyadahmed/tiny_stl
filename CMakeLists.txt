cmake_minimum_required(VERSION 3.16)
project(tiny_stl)

add_library(tiny_stl "tiny_stl.cpp" "tiny_stl.hpp")
set_target_properties(tiny_stl PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)