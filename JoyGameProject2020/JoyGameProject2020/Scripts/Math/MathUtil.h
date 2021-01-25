#pragma once
#include <cmath>

namespace Math {

	const double PI = 3.14159265358979323846;
	const double PIDIV2 = 1.57079632679489661923;
	const double PIDIV4 = 0.785398163397448309616;
	const float EPSILON = 1.0e-5f;

	template <class T>
	T Clamp(T value, T min, T max) {
		return static_cast<T>(fmin(max, fmax(value, min)));
	}

	template <class T>
	T ToRadian(T degree) {
		return degree * static_cast<T>(PI / 180.0);
	}

	template <class T>
	T ToDegree(T radian) {
		return radian * static_cast<T>(180.0 / PI);
	}

	template <class T>
	T Lerp(T start, T end, T t) {
		return (static_cast<T>(1) - t) * start + t * end;
	}

	template <class T>
	T Wrap(T value, T min, T max) {
		const T n = fmod(value - min, max - min);
		return static_cast<T>(n >= 0 ? n + min : n + max);
	}
};

