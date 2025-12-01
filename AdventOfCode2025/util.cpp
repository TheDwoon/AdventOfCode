#ifndef HEADER_UTIL
#define HEADER_UTIL
#include <vector>
#include <cassert>
#include "vec2.cpp"
#include "vec3.cpp"
#include "mat2.cpp"

namespace aoc {
    template<typename T>
    T gcd(T a, T b) {
        T tmp;
        while (b != 0) {
            tmp = b;
            b = a % b;
            a = tmp;
        }

        return a;
    }

    template<typename T>
    T findLCM(const T* data, size_t length) {
        T lcm = 1;
        if (length > 0) {
            lcm = data[0];
            for (unsigned int i = 1; i < length; i++) {
                    lcm = data[i] * lcm / gcd(data[i], lcm);
            }
        }

        return lcm;
    }

    template<typename T>
    int signum(T x) {
        return (x < 0) ? -1 : ((x > 0) ? 1 : 0);
    }

    template<typename T>
    bool inRange(T start, T end, T x) {
        return start <= x && x <= end;
    }

    template<typename T>
    bool inRange(const vec3<T> &start, const vec3<T> &end, const vec3<T> &x) {
        return inRange(start.x, end.x, x.x)
               && inRange(start.y, end.y, x.y)
               && inRange(start.z, end.z, x.z);
    }

    bool hasOverlap(int x1, int x2, int y1, int y2) {
        assert(x1 <= x2);
        assert(y1 <= y2);

        return y1 <= x2 && x1 <= y2;
    }

    struct bounding_box {
        vec3i start;
        vec3i end;
    };

    bool hasOverlap(const bounding_box &a, const bounding_box &b) {
        return hasOverlap(a.start.x, a.end.x, b.start.x, b.end.x)
            && hasOverlap(a.start.y, a.end.y, b.start.y, b.end.y)
            && hasOverlap(a.start.z, a.end.z, b.start.z, b.end.z);
    }

    template<typename T>
    struct map {
        T* buffer;
        const int width;
        const int height;
        const int lineLength;

        [[nodiscard]]
        bool contains(const vec2i& pos) const {
            return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
        }

        [[nodiscard]]
        T& at(const int x, const int y) const {
            assert (x >= 0 && x < width);
            assert (y >= 0 && y < height);
            return buffer[y * lineLength + x];
        }

        [[nodiscard]]
        T& at(const vec2i& pos) const {
            assert (pos.x >= 0 && pos.x < width);
            assert (pos.y >= 0 && pos.y < height);
            return buffer[pos.y * lineLength + pos.x];
        }

        T operator()(const vec2i& pos) const {
            assert (pos.x >= 0 && pos.x < width);
            assert (pos.y >= 0 && pos.y < height);
            return buffer[pos.y * lineLength + pos.x];
        }

        T operator()(const int x, const int y) const {
            assert (x >= 0 && x < width);
            assert (y >= 0 && y < height);
            return buffer[y * lineLength + x];
        }
    };
}
#endif