# TODO: move examples to their own repository.
cmake_minimum_required(VERSION 3.16)
project(Tiny_STL_Examples)

add_subdirectory(tiny_stl)
add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)
