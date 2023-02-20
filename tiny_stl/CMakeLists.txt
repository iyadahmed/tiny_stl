include(FetchContent)

# TODO: distribute fmt and fast_float in "extern" folder instead
#       to remove the need for an internet connection
#       when configuring the project.
FETCHCONTENT_DECLARE(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50
    GIT_PROGRESS TRUE
)
FETCHCONTENT_MAKEAVAILABLE(fmt)

FETCHCONTENT_DECLARE(
    fast_float
    GIT_REPOSITORY https://github.com/fastfloat/fast_float.git
    GIT_TAG 08c47edf570aedfffc7dc1fbbed5cad9be691840
    GIT_PROGRESS TRUE
)
FETCHCONTENT_MAKEAVAILABLE(fast_float)

add_library(tiny_stl "writer.cpp" "reader.cpp" "non_copyable.hpp" "reader_ascii.hpp" "reader_binary.hpp" "writer_ascii.hpp" "writer_binary.hpp")
target_link_libraries(tiny_stl PRIVATE fmt::fmt fast_float)
target_include_directories(tiny_stl PUBLIC "include")
set_target_properties(
    tiny_stl
    PROPERTIES CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
