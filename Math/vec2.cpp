#include "vec2.h"
#include "pch.h"
#include <math.h>

using namespace math;

template<typename T>
vec2<T> vec2<T>::normalized() const {
	double l = length();
	if (l < 0.00000001)
		return vec2(0, 0);
	else
		return vec2(x / l, y / l);
}

template<typename T>
double vec2<T>::length() const {
	return sqrt(x * x + y * y);
}