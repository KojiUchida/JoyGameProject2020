#pragma once

#include "Math/Vector3.h"

class Light {
private:
	Light();
	~Light();
	Light(const Light& other) = delete;
	void operator=(const Light& other) = delete;

public:
	static Light& Instance();
	void SetRotate(const Vector3& rot);
	Vector3 GetRotate();
	Vector3 GetLightVec();

	Vector3 lightColor;

private:
	Vector3 rotate;
	Vector3 lightvec;
};