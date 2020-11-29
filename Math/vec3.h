#pragma once

namespace math {
	template<typename T>
	class vec3
	{
	public:
		T x;
		T y;
		T z;

		vec3(T x, T y, T z) : x(x), y(y), z(z) {}

		const T operator+(const vec3<T>& other) {
			return vec3(x + other.x, y + other.y, z + other.z);
		}

		const T operator-(const vec3<T>& other) {
			return vec3(x - other.x, y - other.y, z - other.z);
		}

		const double operator*(const vec3<T>& other) {
			return x * other.x + y * other.y + z * other.z;
		}

		const T operator*(const double scalar) {
			return vec3(x * scalar, y * scalar, z * scalar);
		}
	};

	typedef vec3<double> vec3d;
	typedef vec3<int> vec3i;
}
