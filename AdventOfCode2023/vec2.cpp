#ifndef HEADER_VEC2
#define HEADER_VEC2
#include <tuple>
#include <ostream>

template<typename T>
struct vec2 {
    T x;
    T y;

    vec2(T x, T y) : x(x), y(y) {

    }

    vec2& operator+=(const vec2<T>& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    friend vec2<T> operator+(vec2<T> lhs, const vec2<T> &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend bool operator<(const vec2<T> &lhs, const vec2<T> &rhs) {
        return std::tie(lhs.y, lhs.x) < std::tie(rhs.y, rhs.x);
    }

    friend bool operator==(const vec2<T> &lhs, const vec2<T> &rhs) {
        return lhs.y == rhs.y && lhs.x == rhs.x;
    }

    friend bool operator!=(const vec2<T> &lhs, const vec2<T> &rhs) {
        return lhs.y != rhs.y || lhs.x != rhs.x;
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec2<T> &v) {
        stream << "(" << v.x << ", " << v.y << ")";
        return stream;
    }
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
#endif
