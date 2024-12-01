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

    typedef map2d_view<int> map2di_view;
    typedef map2d_view<char> map2dc_view;

    template<typename TNode, typename TEdge>
    struct edge;

    template<typename TNode, typename TEdge>
    class node {
    public:
        node() = default;
        node(TNode value): value(value) {

        }

        TNode value;
        std::vector<std::shared_ptr<edge<TNode, TEdge>>> edges_out;
        std::vector<std::shared_ptr<edge<TNode, TEdge>>> edges_in;
    };

    template<typename TNode, typename TEdge>
    struct edge {
        std::shared_ptr<node<TNode, TEdge>> source;
        std::shared_ptr<node<TNode, TEdge>> target;
        TEdge value;
    };

    template<typename TNode = int, typename TEdge = int>
    struct directed_graph {
        std::vector<std::shared_ptr<node<TNode, TEdge>>> nodes;

        std::shared_ptr<node<TNode, TEdge>> addNode(TNode value) {
            std::shared_ptr<node<TNode, TEdge>> n = std::make_shared<node<TNode, TEdge>>(value);
            nodes.push_back(n);
            return n;
        }

        void addEdge(std::shared_ptr<node<TNode, TEdge>>& source, std::shared_ptr<node<TNode, TEdge>>& target, TEdge value) {
            std::shared_ptr<edge<TNode, TEdge>> e = std::make_shared<edge<TNode, TEdge>>(source, target, value);
            source->edges_out.push_back(e);
            target->edges_in.push_back(e);
        }
    };
}
#endif