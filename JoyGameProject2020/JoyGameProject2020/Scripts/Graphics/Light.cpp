#include "Light.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4.h"

Light::Light() :
	lightvec(Vector3(0, -1, 0)),
	lightColor(Vector3(1, 1, 1)) {
}

Light::~Light() {
}

Light& Light::Instance() {
	static Light instance;
	return instance;
}

void Light::SetRotate(const Vector3& rot) {
	rotate = rot;
	lightvec = (Vector3(0, -1, 0) * Matrix4::RotationFromQuaternion(Quaternion::Euler(rot))).Normalize();
}

Vector3 Light::GetRotate() {
	return rotate;
}

Vector3 Light::GetLightVec() {
	return lightvec;
}
