#include <cstdio>
#include "tiny_stl.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        puts("Expected arguments: input.stl output.stl");
        return 1;
    }

    const char* input_filepath = argv[1];
    const char* output_filepath = argv[2];
    auto reader = Tiny_STL::create_reader(input_filepath);
    auto writer = Tiny_STL::create_writer(output_filepath, Tiny_STL::File_Writer::Type::ASCII);
    Tiny_STL::Triangle t;
    while (reader->read_next_triangle(&t)) {
        writer->write_triangle(&t);
    }
    return 0;
}
