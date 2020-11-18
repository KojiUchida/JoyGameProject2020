#include "MathUtility.h"
#define _USE_MATH_DEFINES
#define EPSILON 0.00001
#include <cmath>

float MathUtility::Clamp(float value, float min, float max) {
	return fmin(max, fmax(value, min));
}

float MathUtility::ToRadian(float degree) {
	return degree * (float)M_PI / 180.0f;
}

float MathUtility::ToDegree(float radian) {
	return radian * 180.0f / (float)M_PI;
}

float MathUtility::Lerp(float start, float end, float t) {
	return (1.0f - t) * start + t * end;
}

bool MathUtility::IsZero(float value) {
	return abs(value) < EPSILON;
}

float MathUtility::EpsilonToZero(float value) {
	return IsZero(value) ? 0 : value;
}
