cmake_minimum_required(VERSION 3.16)

project(tiny_stl)

if(MSVC)
  add_link_options($<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
endif()

add_subdirectory("fast_float")
add_subdirectory("src")
add_subdirectory("apps")
