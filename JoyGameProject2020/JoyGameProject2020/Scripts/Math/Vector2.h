#pragma once

struct Vector3;
struct Vector2 {

	float x;
	float y;

	/* �R���X�g���N�^ */
	Vector2();
	Vector2(const float value);
	Vector2(const float x, const float y);

	/* �P�����Z�q */
	Vector2 operator+() const;
	Vector2 operator-() const;

	/* ������Z�q */
	Vector2& operator=(const Vector2& v);
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

	/* ���� */
	static const float Dot(const Vector2& v1, const Vector2& v2);
	/* �O�� */
	static const float Cross(const Vector2& v1, const Vector2& v2);
	/* �p�x */
	static const float Angle(const Vector2& v1, const Vector2& v2);
	/* ���� */
	const float Length() const;
	/* ���K�� */
	const Vector2 Normalize() const;
	/* �ő�l */
	static const Vector2 Max(const Vector2& v1, const Vector2& v2);
	/* �ŏ��l */
	static const Vector2 Min(const Vector2& v1, const Vector2& v2);
	/* �[���x�N�g�� */
	static const Vector2 Zero();
	/* Vector3�֕ϊ� */
	const Vector3 ToVector3() const;
};

/* �񍀉��Z�q */
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v1, const float s);
const Vector2 operator*(const float s, const Vector2& v1);
const Vector2 operator/(const Vector2& v1, const float s);

