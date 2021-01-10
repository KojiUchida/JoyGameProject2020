#pragma once

struct Vector3;
struct Quaternion {

	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(float w, Vector3 v);

	/* �P�����Z�q */
	Quaternion operator+() const;
	Quaternion operator-() const;

	/* ������Z�q */
	Quaternion& operator=(const Quaternion& q);
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(const Quaternion& q);
	Quaternion& operator*=(const float s);
	Quaternion& operator/=(const float s);

	/* �P���l���� */
	const Quaternion Identity();

	static const Quaternion Euler(const Vector3& angle);
	/* ���`��� */
	static const Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
	/* ���ʐ��`��� */
	static const Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	/* ���� */
	static const Quaternion Conjugate(const Quaternion& q);
	/* �ݏ� */
	static const Quaternion Pow(const Quaternion& q, float exponent);

	/* ���� */
	static const float Dot(const Quaternion& q1, const Quaternion& q2);
	/* ���� */
	const float Length() const;
	/* ���K�� */
	const Quaternion Normalize() const;

	/* ��]�p */
	float GetRotationAngle() const;
	/* ��]�� */
	Vector3 GetRotationAxis() const;
	/* �I�C���[�p */
	Vector3 EulerAngles() const;

	void Print();
};

/* �񍀉��Z�q */
const Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator*(const Quaternion& q, const float s);
const Quaternion operator*(const float s, const Quaternion& q);
const Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator/(const Quaternion& q, const float s);