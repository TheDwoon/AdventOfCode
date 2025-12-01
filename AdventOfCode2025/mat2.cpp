#ifndef HEADER_MAT2
#define HEADER_MAT2
#include "vec2.cpp"

/**
 * (m00, m01)
 * (m10, m11)
 *
 * @tparam T
 */
template<typename T>
struct mat2 {
    T m00 {0};
    T m01 {0};
    T m10 {0};
    T m11 {0};

    mat2() : m00(0), m01(0), m10(0), m11(0) {

    }

    mat2(T m00, T m01, T m10, T m11) : m00(m00), m01(m01), m10(m10), m11(m11) {

    }

    mat2(const vec2<T> &col1, const vec2<T> &col2) : m00(col1.x), m01(col2.x), m10(col1.y), m11(col2.y) {

    }

    friend vec2<T> operator*(const mat2<T>& lhs, const vec2<T> &rhs) {
        return { lhs.m00 * rhs.x + lhs.m01 * rhs.y, lhs.m10 * rhs.x + lhs.m11 * rhs.y };
    }
};

typedef mat2<int> mat2i;
typedef mat2<float> mat2f;
typedef mat2<double> mat2d;

#endif