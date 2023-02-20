#pragma once

#include <fmt/os.h>

#include "non_copyable.hpp"
#include "tiny_stl.hpp"

class ASCII_File_Writer : public Tiny_STL::File_Writer, public NonCopyable
{
private:
    fmt::ostream m_file;

public:
    explicit ASCII_File_Writer(const char *filepath);
    ~ASCII_File_Writer() override;
    void write_triangle(const Tiny_STL::Triangle *t) override;
};

ASCII_File_Writer::ASCII_File_Writer(const char *filepath) : m_file(fmt::output_file(filepath))
{
    m_file.print("solid \n");
}

void ASCII_File_Writer::write_triangle(const Tiny_STL::Triangle *t)
{
    m_file.print("facet normal {} {} {}\n"
                 "\touter loop\n"
                 "\t\tvertex {} {} {}\n"
                 "\t\tvertex {} {} {}\n"
                 "\t\tvertex {} {} {}\n"
                 "\tendloop\n"
                 "endfacet\n",

                 t->normal[0], t->normal[1], t->normal[2],
                 t->vertices[0][0], t->vertices[0][1], t->vertices[0][2],
                 t->vertices[1][0], t->vertices[1][1], t->vertices[1][2],
                 t->vertices[2][0], t->vertices[2][1], t->vertices[2][2]);
}

ASCII_File_Writer::~ASCII_File_Writer()
{
    m_file.print("endsolid \n");
}
