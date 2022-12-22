#pragma once

#include <cstdint>
#include <memory>

namespace STL_Mesh_IO {

struct Triangle {
    float normal[3] {};
    float vertices[3][3] {};
    uint16_t attribute_byte_count = 0;
};

class File_Reader {
public:
    virtual bool read_next_triangle(Triangle* t) = 0;

    // NOTE: Abstract class destrctor must be virtual,
    // otherwise, subclasses' destructors won't be called :/
    // https://stackoverflow.com/a/25220259/8094047
    virtual ~File_Reader() { }
};

std::unique_ptr<File_Reader> create_reader(const char* filepath);

}
