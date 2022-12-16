cmake_minimum_required(VERSION 3.16)

project(tiny_stl)

if(MSVC)
  add_link_options($<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
endif()

add_subdirectory("fast_float")
add_library(tiny_stl "tiny_stl.cpp" "tiny_stl.hpp")
target_link_libraries(tiny_stl PUBLIC fast_float)

add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)