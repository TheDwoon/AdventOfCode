#ifndef HEADER_UTIL
#define HEADER_UTIL
#include <vector>
#include "vec2.cpp"
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

    namespace direction {
        static const vec2i UP { 0, -1 };
        static const vec2i DOWN { 0, 1 };
        static const vec2i LEFT { -1, 0};
        static const vec2i RIGHT { 1, 0 };
        static const vec2i DIRECTIONS[4] { UP, DOWN, LEFT, RIGHT };

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
        vec2i topLeft;
        int width{0};
        int height{0};

        map2d() = default;

        map2d(vec2i topLeft, int width, int height) : topLeft(topLeft), width(width), height(height) {
            data.resize(width * height);
        }

        map2d(int width, int height, T init = 0) : width(width), height(height) {
            data.resize(width * height, init);
        }

        [[nodiscard]] bool contains(const vec2i& v) const {
            return contains(v.x, v.y);
        }

        [[nodiscard]] bool contains(int x, int y) const {
            x -= topLeft.x;
            y -= topLeft.y;
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        void resize(const vec2i& newOrigin, int newWidth, int newHeight, T init = 0) {
            std::vector<T> newData;
            newData.resize(newWidth * newHeight, init);

            int dx = topLeft.x - newOrigin.x;
            int dy = topLeft.y - newOrigin.y;

            for (int y = std::max(0, -dy); y < std::min(height, newHeight - dy); y++) {
                for (int x = std::max(0, -dx); x < std::min(width, newWidth - dx); x++) {
                    newData[(y + dy) * newWidth + x + dx] = data[y * width + x];
                }
            }

            topLeft = newOrigin;
            width = newWidth;
            height = newHeight;
            data = newData;
        }

        void extend(vec2i direction, T init = 0) {
            int newWidth = width;
            int newHeight = height;
            vec2i newOrigin = topLeft;

            newWidth += std::abs(direction.x);
            newHeight += std::abs(direction.y);
            if (direction.x < 0) {
                newOrigin.x += direction.x;
            }
            if (direction.y < 0) {
                newOrigin.y += direction.y;
            }

            resize(newOrigin, newWidth, newHeight, init);
        }

        void include(const vec2i &position, int buffer = 0, T init = 0) {
            if (contains(position)) return;

            vec2i delta = position - topLeft;
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

            extend(delta, init);
        }

        void copy(const map2d<T> &other, const vec2i& position, T init = 0) {
            vec2i bottomRight = position + vec2i(other.width - 1, other.height - 1);

            include(position, 0, init);
            include(bottomRight, 0, init);

            for (int y = 0; y < other.height; y++) {
                for (int x = 0; x < other.width; x++) {
                    data[(position.y + y - topLeft.y) * width + position.x + x - topLeft.x] = other.data[y * other.width + x];
                }
            }
        }

        void origin(const vec2i& origin) {
            topLeft -= origin;
        }

        void center() {
            vec2i newCenter {(topLeft.x + width) / 2, (topLeft.y + height) / 2};
            origin(newCenter);
        }

        [[nodiscard]] int minX() const {
            return topLeft.x;
        }

        [[nodiscard]] int maxX() const {
            return topLeft.x + width;
        }

        [[nodiscard]] int minY() const {
            return topLeft.y;
        }

        [[nodiscard]] int maxY() const {
            return topLeft.y + height;
        }

        T& operator()(const vec2i &v) {
            assert(contains(v.x, v.y));
            return data[(v.y - topLeft.y) * width + v.x - topLeft.x];
        }

        const T& operator()(const vec2i &v) const {
            return data[(v.y - topLeft.y) * width + v.x - topLeft.x];
        }

        T& operator()(int x, int y) {
            return data[(y - topLeft.y) * width + x - topLeft.x];
        }

        const T& operator()(int x, int y) const {
            return data[(y - topLeft.y) * width + x - topLeft.x];
        }
    };

    typedef map2d<int> map2di;
    typedef map2d<char> map2dc;

    template<typename T>
    struct map2d_view {
        const map2d<T> *map;
        vec2i offset;

        map2d_view(const map2d<T> *map, const vec2i &offset) : map(map), offset(offset) {
            assert(map != nullptr);
        }

        [[nodiscard]] bool contains(vec2i pos) const {
            pos += offset;
            return map->contains(pos);
        }

        const T& operator()(vec2i pos) const {
            pos += offset;
            return (*map)(pos);
        }
    };
}
#endif