#include <cstdio>
#include <cstring> // for memcmp
#include <string>  // for to_string
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

    // ****************************** Reading ******************************
    class Binary_File_Reader : public File_Reader, public NonCopyable {
    private:
        FILE *m_file = nullptr;

    public:
        explicit Binary_File_Reader(FILE *file) {
            m_file = file;
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
                }
                    // TODO: read normal vector
                else {
                    m_iter++;
                }

                if (vertex_counter >= 3) {
                    // TODO: verify that normal is read
                    return true;
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

    // ****************************** Writing ******************************
    class Binary_File_Writer : public File_Writer, public NonCopyable {
    private:
        FILE *m_file = nullptr;
        uint32_t num_tris = 0;
        static constexpr size_t BINARY_HEADER_SIZE = 80;

    public:
        explicit Binary_File_Writer(const char *filepath) {
            m_file = fopen(filepath, "wb");
            if (m_file == nullptr) {
                throw std::runtime_error("Failed to open file");
            }

            char header[BINARY_HEADER_SIZE] = {};
            fwrite(header, 1, BINARY_HEADER_SIZE, m_file);
            // Write placeholder for number of triangles,
            // so that it can be updated later (after all triangles have been written)
            fwrite(&num_tris, sizeof(uint32_t), 1, m_file);
        }

        void write_triangle(const Triangle *t) override {
            bool success = (fwrite(t->normal, sizeof(float[3]), 1, m_file) == 1);
            success = success && (fwrite(t->vertices, sizeof(float[3][3]), 1, m_file) == 1);
            success = success && (fwrite(&t->attribute_byte_count, sizeof(uint16_t), 1, m_file) == 1);
            if (success) {
                num_tris++;
            }
        }

        ~Binary_File_Writer() override {
            assert(m_file != nullptr);
            fseek(m_file, BINARY_HEADER_SIZE, SEEK_SET);
            fwrite(&num_tris, sizeof(uint32_t), 1, m_file);
            fclose(m_file);
        }
    };

    class ASCII_File_Writer : public File_Writer, public NonCopyable {
    private:
        FILE *m_file = nullptr;

    public:
        explicit ASCII_File_Writer(const char *filepath) {
            m_file = fopen(filepath, "wt");
            if (m_file == nullptr) {
                throw std::runtime_error("Failed to open file");
            }

            fputs("solid \n", m_file);
        }

        void write_triangle(const Triangle *t) override {
            fputs("facet normal", m_file);
            for (auto value: t->normal) {
                fputc(' ', m_file);
                fputs(std::to_string(value).c_str(), m_file);
            }
            fputc('\n', m_file);
            fputs("\touter loop\n", m_file);
            for (auto vertex: t->vertices) {
                fputs("\t\tvertex", m_file);
                for (int i = 0; i < 3; i++) {
                    fputc(' ', m_file);
                    fputs(std::to_string(vertex[i]).c_str(), m_file);
                }
                fputc('\n', m_file);
            }
            fputs("\tend loop\n", m_file);
        }

        ~ASCII_File_Writer() override {
            assert(m_file != nullptr);
            fputs("endsolid \n", m_file);
            fclose(m_file);
        }
    };

    std::unique_ptr<File_Writer> create_writer(const char *filepath, File_Writer::Type type) {
        if (type == File_Writer::Type::ASCII) {
            return std::make_unique<ASCII_File_Writer>(filepath);
        } else if (type == File_Writer::Type::BINARY) {
            return std::make_unique<Binary_File_Writer>(filepath);
        } else {
            throw std::runtime_error("Not implemented");
        }
    }

}