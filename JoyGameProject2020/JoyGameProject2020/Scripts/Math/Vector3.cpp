#include "Vector3.h"
#include <cmath>
#include "Vector2.h"
#include "MathUtil.h"

Vector3::Vector3() :
	x(0),
	y(0),
	z(0) {
}

Vector3::Vector3(const float value) :
	x(value),
	y(value),
	z(value) {
}

Vector3::Vector3(const float x, const float y, const float z) :
	x(x),
	y(y),
	z(z) {
}

Vector3 Vector3::operator+() const {
	return *this;
}

Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator=(const Vector3& vector) {
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	return *this;
}

Vector3& Vector3::operator*=(const float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator==(const Vector3& v) {
	return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=(const Vector3& v) {
	return !(*this == v);
}

const float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	float x = v1.y * v2.z - v1.z * v2.y;
	float y = v1.z * v2.x - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;
	return Vector3(x, y, z);
}

const float Vector3::Angle(const Vector3& v1, const Vector3& v2) {
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float rad = std::atan2(dx, dy);
	return rad;
}

const float Vector3::Length() const {
	return std::sqrtf(x * x + y * y + z * z);
}

const Vector3 Vector3::Normalize() const {
	if (Length() == 0)return *this;
	return *this / Length();
}

const Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) {
	if (v1.Length() > v2.Length())return v1;
	return v2;
}

const Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) {
	if (v1.Length() < v2.Length()) return v1;
	return v2;
}

const Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, const float t) {
	return Vector3(
		Math::Lerp(start.x, end.x, t),
		Math::Lerp(start.y, end.y, t),
		Math::Lerp(start.z, end.z, t)
	);
}

const Vector3 Vector3::Zero() {
	return Vector3(0, 0, 0);
}

const Vector2 Vector3::ToVector2() const {
	return Vector2(x, y);
}

const Vector3 Vector3::Lerp(const Vector3 & start, const Vector3 & end, double t)
{
	auto distance = end - start;
	return start+distance*t;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

const Vector3 operator*(const Vector3& v, const float s) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

const Vector3 operator*(const float s, const Vector3& v) {
	return Vector3(s * v.x, s * v.y, s * v.z);
}

const Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

const Vector3 operator/(const Vector3& v, const float s) {
	if (s == 0)return v;
	return Vector3(v.x / s, v.y / s, v.z / s);
}

