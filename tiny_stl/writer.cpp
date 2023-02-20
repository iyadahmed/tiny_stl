#include <memory>
#include <stdexcept>

#include "tiny_stl.hpp"
#include "writer_ascii.hpp"
#include "writer_binary.hpp"

namespace Tiny_STL
{
    std::unique_ptr<File_Writer> create_writer(const char *filepath, File_Writer::Type type)
    {
        if (type == File_Writer::Type::ASCII)
        {
            return std::make_unique<ASCII_File_Writer>(filepath);
        }
        else if (type == File_Writer::Type::BINARY)
        {
            return std::make_unique<Binary_File_Writer>(filepath);
        }
        else
        {
            throw std::runtime_error("Not implemented");
        }
    }
}
