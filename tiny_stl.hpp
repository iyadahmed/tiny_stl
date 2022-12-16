#pragma once

#include <memory>
#include <stdexcept>
#include <stdint.h>

namespace STL_Mesh_IO {

enum class File_Type {
    BINARY,
    ASCII,
};

struct Triangle {
    float normal[3];
    float vertices[3][3];
    uint16_t attribute_byte_count = 0;
};

class File_Reader {
public:
    virtual bool read_next_triangle(Triangle* t) = 0;
};

std::unique_ptr<File_Reader> create_reader(const char* filepath);
}
