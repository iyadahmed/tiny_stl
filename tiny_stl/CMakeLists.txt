add_subdirectory(extern EXCLUDE_FROM_ALL)

add_library(tiny_stl "writer.cpp" "reader.cpp" "non_copyable.hpp" "reader_ascii.hpp" "reader_binary.hpp" "writer_ascii.hpp" "writer_binary.hpp")
target_link_libraries(tiny_stl PRIVATE fmt::fmt fast_float)
target_include_directories(tiny_stl PUBLIC "include")
set_target_properties(tiny_stl
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
