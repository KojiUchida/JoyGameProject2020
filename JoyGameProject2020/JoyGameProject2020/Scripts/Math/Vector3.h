#pragma once

struct Vector2;
struct Vector3 {

	float x;
	float y;
	float z;

	/* �R���X�g���N�^ */
	Vector3();
	Vector3(const float value);
	Vector3(const float x, const float y, const float z);

	/* �P�����Z�q */
	Vector3 operator+() const;
	Vector3 operator-() const;

	/* ������Z�q */
	Vector3& operator=(const Vector3& v);
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const float s);
	Vector3& operator/=(const float s);

	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);

	/* ���� */
	static const float Dot(const Vector3& v1, const Vector3& v2);
	/* �O�� */
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	/* �p�x */
	static const float Angle(const Vector3& v1, const Vector3& v2);
	/* ���� */
	const float Length() const;
	/* ���K�� */
	const Vector3 Normalize() const;
	/* �ő�l */
	static const Vector3 Max(const Vector3& v1, const Vector3& v2);
	/* �ŏ��l */
	static const Vector3 Min(const Vector3& v1, const Vector3& v2);
	/* �[���x�N�g�� */
	static const Vector3 Zero();
	/* Vector2�֕ϊ� */
	const Vector2 ToVector2() const;

	static const Vector3 Lerp(const Vector3& start, const Vector3& end, double t);
};

/* �񍀉��Z�q */
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, const float s);
const Vector3 operator*(const float s, const Vector3& v1);
const Vector3 operator*(const Vector3& v1, const Vector3& v2);
const Vector3 operator/(const Vector3& v1, const float s);

