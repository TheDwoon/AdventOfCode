#pragma once

namespace math {
	template<typename T>
	class vec2 {
	public:
		T x;
		T y;

		vec2() : vec2(0, 0) {};
		vec2(const T& x, const T& y) : x(x), y(y) {}

		vec2 normalized() const;
		double length() const;		

		const vec2& operator+=(const vec2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		const vec2& operator=(const vec2& other) {
			x = other.x;
			y = other.y;
			return *this;
		}

		const vec2 operator+(const vec2& other) {
			return vec2(x + other.x, y + other.y);
		}

		const vec2 operator-(const vec2& other) {
			return vec2(x - other.x, y - other.y);
		}

		const double operator*(const vec2& other) {
			return x * other.x + y * other.y;
		}

		const vec2 operator*(const double scalar) {
			return vec2(x * scalar, y * scalar);
		}
	};
}