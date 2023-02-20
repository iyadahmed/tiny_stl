#pragma once

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include "non_copyable.hpp"
#include "tiny_stl.hpp"

class Binary_File_Writer : public Tiny_STL::File_Writer, public NonCopyable
{
private:
    FILE *m_file = nullptr;
    uint32_t num_tris = 0;
    static constexpr size_t BINARY_HEADER_SIZE = 80;

public:
    explicit Binary_File_Writer(const char *filepath);
    ~Binary_File_Writer() override;
    void write_triangle(const Tiny_STL::Triangle *t) override;
};

Binary_File_Writer::Binary_File_Writer(const char *filepath)
{
    m_file = fopen(filepath, "wb");
    if (m_file == nullptr)
    {
        throw std::runtime_error("Failed to open file");
    }

    char header[BINARY_HEADER_SIZE] = {};
    fwrite(header, 1, BINARY_HEADER_SIZE, m_file);
    // Write placeholder for number of triangles,
    // so that it can be updated later (after all triangles have been written)
    fwrite(&num_tris, sizeof(uint32_t), 1, m_file);
}

void Binary_File_Writer::write_triangle(const Tiny_STL::Triangle *t)
{
    bool success = (fwrite(t->normal, sizeof(float[3]), 1, m_file) == 1);
    success = success && (fwrite(t->vertices, sizeof(float[3][3]), 1, m_file) == 1);

    uint16_t attribute_byte_count = 0;
    success = success && (fwrite(&attribute_byte_count, sizeof(uint16_t), 1, m_file) == 1);
    if (success)
    {
        num_tris++;
    }
}

Binary_File_Writer::~Binary_File_Writer()
{
    assert(m_file != nullptr);
    fseek(m_file, BINARY_HEADER_SIZE, SEEK_SET);
    fwrite(&num_tris, sizeof(uint32_t), 1, m_file);
    fclose(m_file);
}
