# Using as a CMake dependency

```cmake
FETCHCONTENT_DECLARE(
        tiny_stl
        GIT_REPOSITORY https://github.com/iyadahmed/tiny_stl.git
        GIT_TAG bd01cd8c5fc9aaaa24a22ec7c5c8b980440325e1
)
FETCHCONTENT_MAKEAVAILABLE(tiny_stl)
target_link_libraries(your_target tiny_stl)
```

# Example:

```cpp
#include <cstdio>
#include "tiny_stl.hpp"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        puts("Expected arguments: input.stl binary_output.stl ascii_output.stl");
        return 1;
    }

    const char *input_filepath = argv[1];
    const char *binary_output_filepath = argv[2];
    const char *ascii_output_filepath = argv[3];

    auto reader = Tiny_STL::create_reader(input_filepath);
    auto binary_writer = Tiny_STL::create_writer(binary_output_filepath, Tiny_STL::File_Writer::Type::BINARY);
    auto ascii_writer = Tiny_STL::create_writer(ascii_output_filepath, Tiny_STL::File_Writer::Type::ASCII);

    Tiny_STL::Triangle t;
    while (reader->read_next_triangle(&t)) {
        binary_writer->write_triangle(&t);
        ascii_writer->write_triangle(&t);
    }
    return 0;
}
```
