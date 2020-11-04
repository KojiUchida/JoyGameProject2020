#pragma once

struct Vector3;
struct Vector2 {

	float x;
	float y;

	/* コンストラクタ */
	Vector2();
	Vector2(const float value);
	Vector2(const float x, const float y);

	/* 単項演算子 */
	Vector2 operator+() const;
	Vector2 operator-() const;

	/* 代入演算子 */
	Vector2& operator=(const Vector2& v);
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

	/* 内積 */
	static const float Dot(const Vector2& v1, const Vector2& v2);
	/* 外積 */
	static const float Cross(const Vector2& v1, const Vector2& v2);
	/* 角度 */
	static const float Angle(const Vector2& v1, const Vector2& v2);
	/* 長さ */
	const float Length() const;
	/* 正規化 */
	const Vector2 Normalize() const;
	/* 最大値 */
	static const Vector2 Max(const Vector2& v1, const Vector2& v2);
	/* 最小値 */
	static const Vector2 Min(const Vector2& v1, const Vector2& v2);
	/* ゼロベクトル */
	static const Vector2 Zero();
	/* Vector3へ変換 */
	const Vector3 ToVector3() const;
};

/* 二項演算子 */
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v1, const float s);
const Vector2 operator*(const float s, const Vector2& v1);
const Vector2 operator/(const Vector2& v1, const float s);

