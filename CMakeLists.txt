cmake_minimum_required(VERSION 3.16)
project(tiny_stl)

include(FetchContent)
FETCHCONTENT_DECLARE(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50
        GIT_SHALLOW ON
        GIT_PROGRESS TRUE
)
FETCHCONTENT_MAKEAVAILABLE(fmt)

add_library(tiny_stl "tiny_stl/tiny_stl.cpp" "tiny_stl/tiny_stl.hpp")
target_link_libraries(tiny_stl PRIVATE fmt::fmt)
target_include_directories(tiny_stl PUBLIC "tiny_stl")
set_target_properties(
        tiny_stl
        PROPERTIES CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)
