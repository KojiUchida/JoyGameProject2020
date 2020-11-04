#include "Vector2.h"
#include <cmath>
#include "Vector3.h"


Vector2::Vector2() :
	x(0),
	y(0) {
}

Vector2::Vector2(const float value) :
	x(value),
	y(value) {
}

Vector2::Vector2(const float x, const float y) :
	x(x),
	y(y) {
}

Vector2 Vector2::operator+() const {
	return *this;
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

Vector2& Vector2::operator=(const Vector2& vector) {
	x = vector.x;
	y = vector.y;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& vector) {
	x += vector.x;
	y += vector.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vector) {
	x -= vector.x;
	y -= vector.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

const float Vector2::Dot(const Vector2& v1, const Vector2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

const float Vector2::Cross(const Vector2& v1, const Vector2& v2) {
	return v1.x * v2.y - v2.x * v1.y;
}

const float Vector2::Angle(const Vector2& v1, const Vector2& v2) {
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float rad = std::atan2(dx, dy);
	return rad;
}

const float Vector2::Length() const {
	float x2 = std::pow(x, 2);
	float y2 = std::pow(y, 2);
	return std::sqrtf(x2 + y2);
}

const Vector2 Vector2::Normalize() const {
	if (Length() == 0)return *this;
	return *this / Length();
}

const Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) {
	if (v1.Length() > v2.Length())return v1;
	return v2;
}

const Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) {
	if (v1.Length() < v2.Length()) return v1;
	return v2;
}

const Vector2 Vector2::Zero() {
	return Vector2(0, 0);
}

const Vector3 Vector2::ToVector3() const {
	return Vector3(x, y, 0);
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

const Vector2 operator*(const Vector2& v, const float s) {
	return Vector2(v.x * s, v.y * s);
}

const Vector2 operator*(const float s, const Vector2& v) {
	return Vector2(s * v.x, s * v.y);
}

const Vector2 operator/(const Vector2& v, const float s) {
	if (s == 0)return v;
	return Vector2(v.x / s, v.y / s);
}

