#pragma once

#include <cstdio>
#include <stdexcept>

#include "non_copyable.hpp"
#include "tiny_stl.hpp"

class Binary_File_Reader : public Tiny_STL::File_Reader, public NonCopyable
{
private:
    FILE *m_file = nullptr;

public:
    explicit Binary_File_Reader(FILE *file);
    ~Binary_File_Reader() override;
    bool read_next_triangle(Tiny_STL::Triangle *res) override;
};

Binary_File_Reader::Binary_File_Reader(FILE *file)
{
    m_file = file;
    if (fseek(file, 84, SEEK_SET) != 0)
    {
        throw std::runtime_error("Failed to seek file");
    }
}

Binary_File_Reader::~Binary_File_Reader()
{
    if (m_file)
    {
        fclose(m_file);
    }
}

bool Binary_File_Reader::read_next_triangle(Tiny_STL::Triangle *res)
{
    bool success = (fread(res->normal, sizeof(float[3]), 1, m_file) == 1);
    success = success && (fread(res->vertices, sizeof(float[3][3]), 1, m_file) == 1);

    // Skip "attribute byte count", which is not stored in ASCII format,
    // and is rarely used by binary format
    success = success && (fseek(m_file, sizeof(uint16_t), SEEK_CUR) == 0);
    return success;
}
