#ifndef HEADER_UTIL
#define HEADER_UTIL
#include <vector>
#include "vec2.cpp"
#include "mat2.cpp"

namespace aoc {
    template<typename T>
    T gcd(T a, T b) {
        T tmpA, tmpB;
        while (b != 0) {
            tmpA = a;
            tmpB = b;
            a = tmpB;
            b = tmpA % tmpB;
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
        vec2i origin;
        int width{0};
        int height{0};

        map2d() = default;

        map2d(vec2i origin, int width, int height) : origin(origin), width(width), height(height) {
            data.resize(width * height);
        }

        map2d(int width, int height) : width(width), height(height) {
            data.resize(width * height);
        }

        [[nodiscard]] bool contains(const vec2i& v) const {
            return contains(v.x, v.y);
        }

        [[nodiscard]] bool contains(int x, int y) const {
            x -= origin.x;
            y -= origin.y;
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        void resize(const vec2i& newOrigin, int newWidth, int newHeight) {
            std::vector<T> newData;
            newData.resize(newWidth * newHeight);

            int dx = origin.x - newOrigin.x;
            int dy = origin.y - newOrigin.y;

            for (int y = std::max(0, -dy); y < std::min(height, newHeight - dy); y++) {
                for (int x = std::max(0, -dx); x < std::min(width, newWidth - dx); x++) {
                    newData[(y + dy) * newWidth + x + dx] = data[y * width + x];
                }
            }

            origin = newOrigin;
            width = newWidth;
            height = newHeight;
            data = newData;
        }

        void extend(vec2i direction) {
            int newWidth = width;
            int newHeight = height;
            vec2i newOrigin = origin;

            newWidth += std::abs(direction.x);
            newHeight += std::abs(direction.y);
            if (direction.x < 0) {
                newOrigin.x += direction.x;
            }
            if (direction.y < 0) {
                newOrigin.y += direction.y;
            }

            resize(newOrigin, newWidth, newHeight);
        }

        void include(const vec2i &position, int buffer = 0) {
            if (contains(position)) return;

            vec2i delta = position - origin;
            delta.x += signum(delta.x) * buffer;
            delta.y += signum(delta.y) * buffer;
            if (delta.x >= width)
                delta.x -= width;
            else if (delta.x > 0)
                delta.x = 0;

            if (delta.y >= height)
                delta.y -= height;
            else if (delta.y > 0)
                delta.y = 0;

            extend(delta);
        }

        [[nodiscard]] int minX() const {
            return origin.x;
        }

        [[nodiscard]] int maxX() const {
            return origin.x + width;
        }

        [[nodiscard]] int minY() const {
            return origin.y;
        }

        [[nodiscard]] int maxY() const {
            return origin.y + height;
        }

        T& operator()(const vec2i &v) {
            assert(contains(v.x, v.y));
            return data[(v.y - origin.y) * width + v.x - origin.x];
        }

        const T& operator()(const vec2i &v) const {
            return data[(v.y - origin.y) * width + v.x - origin.x];
        }

        T& operator()(int x, int y) {
            return data[(y - origin.y) * width + x - origin.x];
        }

        const T& operator()(int x, int y) const {
            return data[(y - origin.y) * width + x - origin.x];
        }
    };

    typedef map2d<int> map2di;
    typedef map2d<char> map2dc;
}
#endif