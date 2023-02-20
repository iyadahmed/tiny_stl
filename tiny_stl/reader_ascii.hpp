#pragma once

#include <fast_float/fast_float.h>

#include "non_copyable.hpp"
#include "tiny_stl.hpp"

class ASCII_File_Reader : public Tiny_STL::File_Reader, public NonCopyable
{
private:
    char *m_buffer = nullptr;
    char *m_iter = nullptr;
    size_t m_buffer_size = 0;

public:
    ASCII_File_Reader(FILE *file, size_t file_size);
    ~ASCII_File_Reader() override;
    bool read_next_triangle(Tiny_STL::Triangle *res) override;
};

static const char *skip_control_chars_or_plus(const char *start, const char *end)
{
    while (start < end)
    {
        if (!(*start <= 32 || *start == '+'))
        {
            break;
        }
        start++;
    }
    return start;
}

static void read_float3(float out[3], const char *buf, const char *endptr)
{
    // TODO: error checking
    buf = skip_control_chars_or_plus(buf, endptr);
    buf = fast_float::from_chars(buf, endptr, out[0]).ptr;

    buf = skip_control_chars_or_plus(buf, endptr);
    buf = fast_float::from_chars(buf, endptr, out[1]).ptr;

    buf = skip_control_chars_or_plus(buf, endptr);
    fast_float::from_chars(buf, endptr, out[2]);
}

ASCII_File_Reader::ASCII_File_Reader(FILE *file, size_t file_size)
{
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        throw std::runtime_error("Failed to seek file");
    }

    if (file_size < 6)
    {
        throw std::runtime_error("File too short");
    }

    m_buffer_size = file_size;
    m_iter = m_buffer = new char[file_size];
    if (fread(m_buffer, file_size, 1, file) != 1)
    {
        fclose(file);
        throw std::runtime_error("Failed to read from file");
    }
    fclose(file);
}

ASCII_File_Reader::~ASCII_File_Reader()
{
    delete[] m_buffer;
}

bool ASCII_File_Reader::read_next_triangle(Tiny_STL::Triangle *res)
{
    int vertex_counter = 0;
    int normal_counter = 0;
    const char *endptr = m_buffer + m_buffer_size;
    while (m_iter < (endptr - 6))
    {
        if (memcmp(m_iter, "vertex", 6) == 0)
        {
            m_iter += 6;
            read_float3(res->vertices[vertex_counter], m_iter, endptr);
            vertex_counter++;
        }
        else if (memcmp(m_iter, "normal", 6) == 0)
        {
            m_iter += 6;
            read_float3(res->normal, m_iter, endptr);
            normal_counter++;
        }
        else
        {
            m_iter++;
        }

        if (vertex_counter >= 3)
        {
            // Normals should have been read before triangle vertices
            // and only one normal should have been read
            return (normal_counter == 1);
        }
    }

    return false;
}
