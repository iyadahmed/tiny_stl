#include <stdexcept>
#include <stdio.h>
#include <string.h> // for memcmp

#include "tiny_stl.hpp"

namespace STL_Mesh_IO {

class Binary_File_Reader : public File_Reader {
private:
    FILE* file = NULL;

public:
    // Make instances non-copyable
    Binary_File_Reader(const Binary_File_Reader&) = delete;
    Binary_File_Reader& operator=(const Binary_File_Reader&) = delete;

    Binary_File_Reader(FILE* file)
    {
        this->file = file;
        if (fseek(file, 84, SEEK_SET) != 0) {
            throw std::runtime_error("Failed to seek file");
        }
    }

    ~Binary_File_Reader()
    {
        if (file) {
            fclose(file);
        }
    }

    bool read_next_triangle(Triangle* res)
    {
        bool success = true;
        success = success && (fread(res->normal, sizeof(float[3]), 1, file) == 1);
        success = success && (fread(res->vertices, sizeof(float[3][3]), 1, file) == 1);
        success = success && (fread(&res->attribute_byte_count, sizeof(uint16_t), 1, file) == 1);
        return success;
    }
};

class ASCII_File_Reader : public File_Reader {
private:
    char* buffer = NULL;
    char* iter = NULL;
    long buffer_size = 0;

public:
    // Make instances non-copyable
    ASCII_File_Reader(const ASCII_File_Reader&) = delete;
    ASCII_File_Reader& operator=(const ASCII_File_Reader&) = delete;

    ASCII_File_Reader(FILE* file, long file_size)
    {
        if (fseek(file, 0, SEEK_SET) != 0) {
            throw std::runtime_error("Failed to seek file");
        }

        if (file_size < 6) {
            throw std::runtime_error("File too short");
        }

        buffer_size = file_size;
        iter = buffer = new char[file_size];
        if (fread(buffer, file_size, 1, file) != 1) {
            fclose(file);
            throw std::runtime_error("Failed to read from file");
        }
        fclose(file);
    }

    bool read_next_triangle(Triangle* res)
    {
        int vertex_counter = 0;
        while (iter < (buffer + buffer_size - 6)) {
            if (memcmp(iter, "vertex", 6) == 0) {
                iter += 6;

                char* endptr = NULL;
                // TODO: strtof error checking
                res->vertices[vertex_counter][0] = strtof(iter, &endptr);
                res->vertices[vertex_counter][1] = strtof(endptr, &endptr);
                res->vertices[vertex_counter][2] = strtof(endptr, &endptr);
                vertex_counter++;

                if (vertex_counter >= 3) {
                    // TODO: verify that normal is read
                    return true;
                }
            }
            // TODO: read normal vector
            else {
                iter++;
            }
        }

        return false;
    }
};

std::unique_ptr<File_Reader> create_reader(const char* filepath)
{
    FILE* file = fopen(filepath, "rb");

    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    if (fseek(file, 80, SEEK_SET) != 0) {
        throw std::runtime_error("Failed to seek file");
    }

    uint32_t num_tris = 0;
    if (fread(&num_tris, sizeof(uint32_t), 1, file) != 1) {
        throw std::runtime_error("Failed to read from file");
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        throw std::runtime_error("Failed to seek file");
    }

    long file_size = ftell(file);

    if (file_size == -1L) {
        throw std::runtime_error("Failed to get file size");
    }

    if (file_size == (84 + num_tris * 50)) {
        return std::make_unique<Binary_File_Reader>(file);
    } else {
        return std::make_unique<ASCII_File_Reader>(file, file_size);
    }
}

}