#pragma once
#include <math.h>

template<typename T>
class vec2 {
public:
    T x;
    T y;

    vec2() : x(0), y(0) {}
    vec2(T x, T y) : x(x), y(y) {}

    void addI(const vec2& other) {
        x += other.x;
        y += other.y;
    }

    void subI(const vec2& other) {
        x -= other.x;
        y -= other.y;
    }

    vec2 rotate(double angle) const {
        vec2 a {cos(angle), sin(angle)};
        vec2 b {-sin(angle), cos(angle)};

        return a * x + b * y;
    }

    vec2 normalize() const {
        const double l = length();
        if (x == 0 && y == 0)
            return vec2();
        else
            return vec2(x / l, y / l);
    }

    double length() const {
        return sqrt(x * x + y * y);
    }

    vec2& operator+=(const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    vec2& operator=(const vec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }

    double operator*(const vec2& other) const {
        return x * other.x + y * other.y;
    }

    vec2 operator*(double scalar) const {
        return vec2(x * scalar, y * scalar);
    }

    vec2 operator*(int scalar) const {
        return vec2(x * scalar, y * scalar);
    }

    bool operator<(const vec2& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

typedef vec2<int> vec2i;
typedef vec2<double> vec2d;