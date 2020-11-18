#pragma once

namespace MathUtility {
	float Clamp(float value, float min, float max);
	float ToRadian(float degree);
	float ToDegree(float radian);
	float Lerp(float start, float end, float t);
	bool IsZero(float value);
	float EpsilonToZero(float value);
};

