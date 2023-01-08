#include <cstdio>
#include "tiny_stl.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Not enough args");
        return 1;
    }

    const char* filepath = argv[1];
    auto reader = Tiny_STL::create_reader(filepath);
    Tiny_STL::Triangle t;
    int n = 0;
    while (reader->read_next_triangle(&t)) {
        n++;
    }
    printf("Number of triangle: %d\n", n);
    return 0;
}
