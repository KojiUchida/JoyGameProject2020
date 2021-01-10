#include "Quaternion.h"
#include "Vector3.h"
#include "MathUtil.h"
#include <iostream>

Quaternion::Quaternion() :
	w(1),
	x(0),
	y(0),
	z(0) {
}

Quaternion::Quaternion(float w, float x, float y, float z) :
	w(w),
	x(x),
	y(y),
	z(z) {
}

Quaternion::Quaternion(float w, Vector3 v) :
	w(w),
	x(v.x),
	y(v.y),
	z(v.z) {
}

Quaternion Quaternion::operator+() const {
	return *this;
}

Quaternion Quaternion::operator-() const {
	return Quaternion(-w, -x, -y, -z);
}

Quaternion& Quaternion::operator=(const Quaternion& q) {
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& q) {
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& q) {
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q) {
	w = w * q.w - x * q.x - y * q.y - z * q.z;
	x = w * q.x + x * q.w + z * q.y - y * q.z;
	y = w * q.y + y * q.w + x * q.z - z * q.x;
	z = w * q.z + z * q.w + y * q.x - x * q.y;
	return *this;
}

Quaternion& Quaternion::operator*=(const float s) {
	w *= s;
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Quaternion& Quaternion::operator/=(const float s) {
	w /= s;
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Quaternion Quaternion::Identity() {
	return Quaternion();
}

const Quaternion Quaternion::Euler(const Vector3& angle) {
	float radX = MathUtil::ToRadian(angle.x);
	float radY = MathUtil::ToRadian(angle.y);
	float radZ = MathUtil::ToRadian(angle.z);

	double cosRoll = cos(radX / 2.0);
	double sinRoll = sin(radX / 2.0);
	double cosPitch = cos(radY / 2.0);
	double sinPitch = sin(radY / 2.0);
	double cosYaw = cos(radZ / 2.0);
	double sinYaw = sin(radZ / 2.0);

	auto result = Quaternion(
		cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw,
		sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
		cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
		cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw
	);

	return result;
}

const Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t) {
	auto result = Quaternion(
		MathUtil::Lerp(q1.w, q2.w, t),
		MathUtil::Lerp(q1.x, q2.x, t),
		MathUtil::Lerp(q1.y, q2.y, t),
		MathUtil::Lerp(q1.z, q2.z, t)
	);
	return result.Normalize();
}

const Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	if (t <= 0.0f) return q1;
	if (t >= 1.0f) return q2;

	float cosOmega = Dot(q1, q2);
	_ASSERT_EXPR(cosOmega < 1.1f, "");

	float q2w = q1.w;
	float q2x = q1.x;
	float q2y = q1.y;
	float q2z = q1.z;

	if (cosOmega < 0.0f) {
		q2w = -q2w;
		q2x = -q2x;
		q2y = -q2y;
		q2z = -q2z;
		cosOmega = -cosOmega;
	}

	float k1;
	float k2;
	if (cosOmega > 0.9999f) {
		k1 = 1.0f - t;
		k2 = t;
	} else {
		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k1 = sin((1.0f - t) * omega * omega) * oneOverSinOmega;
		k2 = sin(t * omega * omega) * oneOverSinOmega;
	}

	Quaternion result;
	result.x = k1 * q1.x + k2 * q2x;
	result.y = k1 * q1.y + k2 * q2y;
	result.z = k1 * q1.z + k2 * q2z;
	result.w = k1 * q1.w + k2 * q2w;

	return result.Normalize();
}

const Quaternion Quaternion::Conjugate(const Quaternion& q) {
	return Quaternion(q.w, -q.x, -q.y, -q.z);
}

Vector3 Quaternion::EulerAngles() const {
	Vector3 euler = Vector3::Zero();
	auto q = *this;
	float sp = -2.0f * (q.y * q.z - q.w * q.x);
	if (abs(sp) >= 1.0f) {
		euler.x = (float)(MathUtil::PI / 2.0f) * sp;
		euler.y = atan2f(-q.x * q.z + q.w * q.y, 0.5f - q.y * q.y - q.z * q.z);
		euler.z = 0.0f;
	} else {
		euler.x = asinf(sp);
		euler.y = atan2f(q.x * q.z + q.w * q.y, 0.5f - q.x * q.x - q.y * q.y);
		euler.z = atan2f(q.x * q.y + q.w * q.z, 0.5f - q.x * q.x - q.z * q.z);
	}
	euler *= MathUtil::ToDegree(1.0f);

	return euler;
}

const Quaternion Quaternion::Pow(const Quaternion& q, float exponent) {
	if (fabs(q.w) > 0.9999f) {
		return q;
	}
	float alpha = acos(q.w);
	float newAlpha = alpha * exponent;

	Quaternion result;
	result.w = cos(newAlpha);

	float mult = sin(newAlpha) / sin(alpha);
	result.x = q.x * mult;
	result.y = q.y * mult;
	result.z = q.z * mult;

	return result.Normalize();
}

const float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

const float Quaternion::Length() const {
	return std::sqrtf(Dot(*this, *this));
}

const Quaternion Quaternion::Normalize() const {
	if (Length() == 0)return *this;
	return *this / Length();
}

float Quaternion::GetRotationAngle() const {
	return acos(w) * 2.0f;
}

Vector3 Quaternion::GetRotationAxis() const {
	return Vector3(x, y, z) / Length();
}

void Quaternion::Print() {
	std::cout << "w = " << w << ", x = " << x << ", y = " << y << ", z = " << z << std::endl;
}

const Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

const Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

const Quaternion operator*(const Quaternion& q, const float s) {
	return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);
}

const Quaternion operator*(const float s, const Quaternion& q) {
	return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);
}

const Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.w * q2.w, q1.x * q2.x, q1.y * q2.y, q1.z * q2.z);
}

const Quaternion operator/(const Quaternion& q, const float s) {
	return Quaternion(q.w / s, q.x / s, q.y / s, q.z / s);
}
