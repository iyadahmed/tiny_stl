#include <cstdio>
#include <cstring> // for memcmp
#include <memory>
#include <stdexcept>
#include <cassert>

#include "tiny_stl.hpp"

namespace Tiny_STL {

    class NonCopyable {
    public:
        NonCopyable() = default;

        ~NonCopyable() = default;

        NonCopyable(const NonCopyable &) = delete;

        NonCopyable &operator=(const NonCopyable &) = delete;
    };

    class Binary_File_Reader : public File_Reader, public NonCopyable {
    private:
        FILE *m_file = nullptr;

    public:
        explicit Binary_File_Reader(FILE *file) {
            this->m_file = file;
            if (fseek(file, 84, SEEK_SET) != 0) {
                throw std::runtime_error("Failed to seek file");
            }
        }

        ~Binary_File_Reader() override {
            if (m_file) {
                fclose(m_file);
            }
        }

        bool read_next_triangle(Triangle *res) override {
            bool success = (fread(res->normal, sizeof(float[3]), 1, m_file) == 1);
            success = success && (fread(res->vertices, sizeof(float[3][3]), 1, m_file) == 1);
            success = success && (fread(&res->attribute_byte_count, sizeof(uint16_t), 1, m_file) == 1);
            return success;
        }
    };

    class ASCII_File_Reader : public File_Reader, public NonCopyable {
    private:
        char *m_buffer = nullptr;
        char *m_iter = nullptr;
        size_t m_buffer_size = 0;

    public:
        ASCII_File_Reader(FILE *file, size_t file_size) {
            if (fseek(file, 0, SEEK_SET) != 0) {
                throw std::runtime_error("Failed to seek file");
            }

            if (file_size < 6) {
                throw std::runtime_error("File too short");
            }

            m_buffer_size = file_size;
            m_iter = m_buffer = new char[file_size];
            if (fread(m_buffer, file_size, 1, file) != 1) {
                fclose(file);
                throw std::runtime_error("Failed to read from file");
            }
            fclose(file);
        }

        ~ASCII_File_Reader() override {
            delete[] m_buffer;
        }

        bool read_next_triangle(Triangle *res) override {
            int vertex_counter = 0;
            while (m_iter < (m_buffer + m_buffer_size - 6)) {
                if (memcmp(m_iter, "vertex", 6) == 0) {
                    m_iter += 6;

                    char *endptr = nullptr;
                    // TODO: strtof error checking
                    res->vertices[vertex_counter][0] = strtof(m_iter, &endptr);
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
                    m_iter++;
                }
            }

            return false;
        }
    };

    std::unique_ptr<File_Reader> create_reader(const char *filepath) {
        FILE *file = fopen(filepath, "rb");

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

        assert(file_size >= 0);
        if ((size_t) file_size == (84 + num_tris * 50)) {
            return std::make_unique<Binary_File_Reader>(file);
        } else {
            return std::make_unique<ASCII_File_Reader>(file, file_size);
        }
    }

}