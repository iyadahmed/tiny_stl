cmake_minimum_required(VERSION 3.16)
project(tiny_stl)

add_library(tiny_stl "tiny_stl.cpp" "tiny_stl.hpp")
set_target_properties(
        tiny_stl
        PROPERTIES CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

# https://airbus-seclab.github.io/c-compiler-security/
# TODO: Enable stricter flags for clang
if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(
            tiny_stl
            PRIVATE -Wdouble-promotion
            -fno-common
            -O2
            -Werror
            -Wall
            -Wextra
            -Wpedantic
            -Wformat=2
            -Wformat-overflow=2
            -Wformat-truncation=2
            -Wformat-security
            -Wnull-dereference
            -Wstack-protector
            -Wtrampolines
            -Walloca
            -Wvla
            -Warray-bounds=2
            -Wimplicit-fallthrough=3
            -Wshift-overflow=2
            -Wcast-qual
            -Wstringop-overflow=4
            -Wconversion
            -Wlogical-op
            -Wduplicated-cond
            -Wduplicated-branches
            -Wformat-signedness
            -Wshadow
            -Wstrict-overflow=4
            -Wundef
            -Wswitch-default
            -Wswitch-enum
            -Wstack-usage=1000000
            -Wcast-align=strict
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=3
            #            -fstack-protector-strong
            -fstack-clash-protection
            -fPIE
            -fsanitize=bounds
            -fsanitize-undefined-trap-on-error
            -Wl,-z,relro
            -Wl,-z,now
            -Wl,-z,noexecstack
            -Wl,-z,separate-code)
elseif (MSVC)
    target_compile_options(tiny_stl PRIVATE /Wall /guard:cf /guard:ehcont)
endif ()

add_executable(example "example.cpp")
target_link_libraries(example tiny_stl)
