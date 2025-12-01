#ifndef HEADER_VEC3
#define HEADER_VEC3
#include <tuple>
#include <ostream>

template<typename T>
struct vec3 {
    T x;
    T y;
    T z;

    vec3() : x(0), y(0), z(0) {

    }

    vec3(T x, T y, T z) : x(x), y(y), z(z) {

    }

    static vec3<T> min(const vec3<T>& a, const vec3<T>& b) {
        vec3<T> v;
        v.x = std::min(a.x, b.x);
        v.y = std::min(a.y, b.y);
        v.z = std::min(a.z, b.z);

        return v;
    }

    static vec3<T> max(const vec3<T>& a, const vec3<T>& b) {
        vec3<T> v;
        v.x = std::max(a.x, b.x);
        v.y = std::max(a.y, b.y);
        v.z = std::max(a.z, b.z);

        return v;
    }

    vec3& operator+=(const vec3<T>& other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    vec3& operator+=(const T& other) {
        x += other;
        y += other;
        z += other;

        return *this;
    }

    vec3& operator-=(const vec3<T>& other) {
        x -= other.x;
        y -= other.y;
        y -= other.z;

        return *this;
    }

    vec3& operator*=(const T& other) {
        x *= other;
        y *= other;
        z *= other;

        return *this;
    }

    friend vec3<T> operator+(vec3<T> lhs, const vec3<T> &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend vec3<T> operator+(vec3<T> lhs, const T& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend vec3<T> operator-(vec3<T> lhs, const vec3<T> &rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend vec3<T> operator*(vec3<T> lhs, const T& rhs) {
        lhs.x *= rhs;
        lhs.y *= rhs;
        lhs.z *= rhs;
        return lhs;
    }

    friend vec3<T> operator*(const T& rhs, vec3<T> lhs) {
        lhs.x *= rhs;
        lhs.y *= rhs;
        lhs.z *= rhs;
        return lhs;
    }

    friend bool operator<(const vec3<T> &lhs, const vec3<T> &rhs) {
        return std::tie(lhs.y, lhs.x, lhs.z) < std::tie(rhs.y, rhs.x, rhs.z);
    }

    friend bool operator==(const vec3<T> &lhs, const vec3<T> &rhs) {
        return lhs.y == rhs.y && lhs.x == rhs.x && lhs.z == rhs.z;
    }

    friend bool operator!=(const vec3<T> &lhs, const vec3<T> &rhs) {
        return lhs.y != rhs.y || lhs.x != rhs.x || lhs.z != rhs.z;
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec3<T> &v) {
        stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return stream;
    }
};

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
#endif
