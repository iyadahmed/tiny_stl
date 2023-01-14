#pragma once

#include <cstdint>
#include <memory>

namespace Tiny_STL {

    struct Triangle {
        float normal[3]{};
        float vertices[3][3]{};
    };

    class File_Reader {
    public:
        virtual bool read_next_triangle(Triangle *t) = 0;

        // NOTE: Abstract class destructor must be virtual,
        // otherwise, subclasses' destructors won't be called :/
        // https://stackoverflow.com/a/25220259/8094047
        virtual ~File_Reader() = default;
    };

    class File_Writer {
    public:
        enum class Type {
            BINARY, ASCII
        };

        virtual void write_triangle(const Triangle *t) = 0;

        virtual ~File_Writer() = default;
    };

    std::unique_ptr<File_Reader> create_reader(const char *filepath);
    std::unique_ptr<File_Writer> create_writer(const char *filepath, File_Writer::Type type);

}
