#include <cassert>
#include <cstdio>
#include <memory>
#include <stdexcept>

#include "reader_ascii.hpp"
#include "reader_binary.hpp"
#include "tiny_stl.hpp"

namespace Tiny_STL
{
    std::unique_ptr<File_Reader> create_reader(const char *filepath)
    {
        FILE *file = fopen(filepath, "rb");

        if (!file)
        {
            throw std::runtime_error("Failed to open file");
        }

        if (fseek(file, 80, SEEK_SET) != 0)
        {
            throw std::runtime_error("Failed to seek file");
        }

        uint32_t num_tris = 0;
        if (fread(&num_tris, sizeof(uint32_t), 1, file) != 1)
        {
            throw std::runtime_error("Failed to read from file");
        }

        if (fseek(file, 0, SEEK_END) != 0)
        {
            throw std::runtime_error("Failed to seek file");
        }

        long file_size = ftell(file);

        if (file_size == -1L)
        {
            throw std::runtime_error("Failed to get file size");
        }

        assert(file_size >= 0);
        if ((size_t)file_size == (84 + num_tris * 50))
        {
            return std::make_unique<Binary_File_Reader>(file);
        }
        else
        {
            return std::make_unique<ASCII_File_Reader>(file, file_size);
        }
    }
}
