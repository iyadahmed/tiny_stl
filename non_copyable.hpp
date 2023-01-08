#pragma once

namespace Tiny_STL {

    class NonCopyable {
    public:
        NonCopyable() = default;

        ~NonCopyable() = default;

        NonCopyable(const NonCopyable &) = delete;

        NonCopyable &operator=(const NonCopyable &) = delete;
    };

}