#pragma once

struct Vector2;
struct Vector3 {

	float x;
	float y;
	float z;

	/* コンストラクタ */
	Vector3();
	Vector3(const float value);
	Vector3(const float x, const float y, const float z);

	/* 単項演算子 */
	Vector3 operator+() const;
	Vector3 operator-() const;

	/* 代入演算子 */
	Vector3& operator=(const Vector3& v);
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const float s);
	Vector3& operator/=(const float s);

	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);

	/* 内積 */
	static const float Dot(const Vector3& v1, const Vector3& v2);
	/* 外積 */
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	/* 角度 */
	static const float Angle(const Vector3& v1, const Vector3& v2);
	/* 長さ */
	const float Length() const;
	/* 正規化 */
	const Vector3 Normalize() const;
	/* 最大値 */
	static const Vector3 Max(const Vector3& v1, const Vector3& v2);
	/* 最小値 */
	static const Vector3 Min(const Vector3& v1, const Vector3& v2);
	/* ゼロベクトル */
	static const Vector3 Zero();
	/* Vector2へ変換 */
	const Vector2 ToVector2() const;

	static const Vector3 Lerp(const Vector3& start, const Vector3& end, double t);
};

/* 二項演算子 */
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, const float s);
const Vector3 operator*(const float s, const Vector3& v1);
const Vector3 operator*(const Vector3& v1, const Vector3& v2);
const Vector3 operator/(const Vector3& v1, const float s);

