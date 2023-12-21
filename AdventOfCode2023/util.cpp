#ifndef HEADER_UTIL
#define HEADER_UTIL
#include <vector>
#include "vec2.cpp"

template<typename T>
struct map2d{
    std::vector<T> data;
    int width {0};
    int height {0};

    T& operator()(const vec2i &v) {
        return data[v.y * width + v.x];
    }

    T& operator()(int x, int y) {
        return data[y * width + x];
    }
};

typedef map2d<int> map2di;
typedef map2d<char> map2dc;

#endif