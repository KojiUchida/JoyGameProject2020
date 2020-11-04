#include "MathUtility.h"
#define _USE_MATH_DEFINES
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
