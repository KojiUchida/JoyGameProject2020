#pragma once
#include "Quaternion.h"

struct Vector3;
struct Quaternion;
struct Matrix4 {

	typedef float Degree;
	typedef float Radian;

	float m[4][4];

	/* コンストラクタ */
	Matrix4();
	/* コンストラクタ */
	Matrix4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	/* 代入演算子 */
	Matrix4& operator=(const Matrix4& mat);

	/* 単位行列 */
	static const Matrix4 Identity();
	/* 拡大・縮小行列 */
	static const Matrix4 Scale(const Vector3& vec);
	/* 平行移動行列 */
	static const Matrix4 Translate(const Vector3& vec);

	static const Matrix4 RotationFromQuaternion(const Quaternion& q);
	/* X軸回転行列 */
	static const Matrix4 Rotation(const Vector3& x, const Vector3& y, const Vector3& z);
	/* X軸回転行列 */
	static const Matrix4 RotateX(const Radian rot);
	/* Y軸回転行列 */
	static const Matrix4 RotateY(const Radian rot);
	/* Z軸回転行列 */
	static const Matrix4 RotateZ(const Radian rot);
	/* RollPitchYaw角による回転行列 */
	static const Matrix4 RotateRollPitchYaw(const Vector3& vec);
	static const Matrix4 RotateRollPitchYaw(const Radian pitch, const  Radian yaw, const  Radian roll);

	/* 透視変換行列 */
	static const Matrix4 Perspective(
		const Degree fov, const float aspect, const float near, const float far);
	/* 視野変換行列 */
	static const Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/* 転置行列 */
	Matrix4 transpose() const;
	/* 逆行列 */
	Matrix4 Inverse() const;
	/* 座標変換(ベクトルと行列の掛け算) */
	Vector3 transform(const Vector3& vec, const Matrix4& mat) const;
	/* 座標変換(法線ベクトル用、移動は行わない) */
	Vector3 transformNormal(const Vector3& vec, const Matrix4& mat) const;
};

/* 二項演算子 */
const Matrix4 operator+(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator-(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat, const float scalar);
const Matrix4 operator*(const float scalar, const Matrix4& mat);
const Vector3 operator*(const Vector3& vec, const Matrix4& mat);

