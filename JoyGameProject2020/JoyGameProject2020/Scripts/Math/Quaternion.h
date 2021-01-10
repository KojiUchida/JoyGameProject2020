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

	/* ’P€‰‰Zq */
	Quaternion operator+() const;
	Quaternion operator-() const;

	/* ‘ã“ü‰‰Zq */
	Quaternion& operator=(const Quaternion& q);
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(const Quaternion& q);
	Quaternion& operator*=(const float s);
	Quaternion& operator/=(const float s);

	/* P“™lŒ³” */
	const Quaternion Identity();

	static const Quaternion Euler(const Vector3& angle);
	/* üŒ`•âŠÔ */
	static const Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
	/* ‹…–ÊüŒ`•âŠÔ */
	static const Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	/* ‹¤–ğ */
	static const Quaternion Conjugate(const Quaternion& q);
	/* —İæ */
	static const Quaternion Pow(const Quaternion& q, float exponent);

	/* “àÏ */
	static const float Dot(const Quaternion& q1, const Quaternion& q2);
	/* ’·‚³ */
	const float Length() const;
	/* ³‹K‰» */
	const Quaternion Normalize() const;

	/* ‰ñ“]Šp */
	float GetRotationAngle() const;
	/* ‰ñ“]² */
	Vector3 GetRotationAxis() const;
	/* ƒIƒCƒ‰[Šp */
	Vector3 EulerAngles() const;

	void Print();
};

/* “ñ€‰‰Zq */
const Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator*(const Quaternion& q, const float s);
const Quaternion operator*(const float s, const Quaternion& q);
const Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator/(const Quaternion& q, const float s);