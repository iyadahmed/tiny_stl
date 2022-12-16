cmake_minimum_required(VERSION 3.22)

project(tiny_stl)

if(MSVC)
  add_link_options($<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
endif()

add_library(tiny_stl "tiny_stl.cpp" "tiny_stl.hpp")

add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)