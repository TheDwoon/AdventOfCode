#ifndef HEADER_UTIL
#define HEADER_UTIL
#include <vector>
#include "vec2.cpp"
#include "mat2.cpp"

namespace aoc {
    namespace direction {
        static const vec2i UP { 0, -1 };
        static const vec2i DOWN { 0, 1 };
        static const vec2i LEFT { -1, 0};
        static const vec2i RIGHT { 1, 0 };

        const static mat2i TURN_RIGHT {0, -1, 1, 0};
        const static mat2i TURN_LEFT {0, 1, -1, 0};

        static const vec2i NORTH { 0, -1 };
        static const vec2i SOUTH { 0, 1 };
        static const vec2i WEST { -1, 0};
        static const vec2i EAST { 1, 0 };

    }

    template<typename T>
    struct map2d {
        std::vector<T> data;
        int width{0};
        int height{0};

        map2d() = default;

        map2d(int width, int height) : width(width), height(height) {
            data.resize(width * height);
        }

        [[nodiscard]] bool contains(const vec2i& v) const {
            return contains(v.x, v.y);
        }

        [[nodiscard]] bool contains(int x, int y) const {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        T &operator()(const vec2i &v) {
            return data[v.y * width + v.x];
        }

        const T &operator()(const vec2i &v) const {
            return data[v.y * width + v.x];
        }

        T &operator()(int x, int y) {
            return data[y * width + x];
        }

        const T &operator()(int x, int y) const {
            return data[y * width + x];
        }
    };

    typedef map2d<int> map2di;
    typedef map2d<char> map2dc;
}
#endif