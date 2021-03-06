#pragma once

struct Vector3;
struct Quaternion;
struct Matrix4 {

	typedef float Degree;
	typedef float Radian;

	float m[4][4];

	/* RXgN^ */
	Matrix4();
	/* RXgN^ */
	Matrix4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	/* ãüZq */
	Matrix4& operator=(const Matrix4& mat);

	/* PÊsñ */
	static const Matrix4 Identity();
	/* gåEk¬sñ */
	static const Matrix4 Scale(const Vector3& vec);
	/* ½sÚ®sñ */
	static const Matrix4 Translate(const Vector3& vec);

	static const Matrix4 RotationFromQuaternion(const Quaternion& q);
	/* X²ñ]sñ */
	static const Matrix4 Rotation(const Vector3& x, const Vector3& y, const Vector3& z);
	/* X²ñ]sñ */
	static const Matrix4 RotateX(const Radian rot);
	/* Y²ñ]sñ */
	static const Matrix4 RotateY(const Radian rot);
	/* Z²ñ]sñ */
	static const Matrix4 RotateZ(const Radian rot);
	/* RollPitchYawpÉæéñ]sñ */
	static const Matrix4 RotateRollPitchYaw(const Vector3& vec);
	static const Matrix4 RotateRollPitchYaw(const Radian pitch, const  Radian yaw, const  Radian roll);

	/* §Ï·sñ */
	static const Matrix4 Perspective(
		const Degree fov, const float aspect, const float near, const float far);
	/* ìÏ·sñ */
	static const Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/* ]usñ */
	Matrix4 transpose() const;
	/* tsñ */
	Matrix4 Inverse() const;
	/* ÀWÏ·(xNgÆsñÌ|¯Z) */
	Vector3 transform(const Vector3& vec, const Matrix4& mat) const;
	/* ÀWÏ·(@üxNgpAÚ®ÍsíÈ¢) */
	Vector3 transformNormal(const Vector3& vec, const Matrix4& mat) const;
};

/* ñZq */
const Matrix4 operator+(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator-(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat, const float scalar);
const Matrix4 operator*(const float scalar, const Matrix4& mat);
const Vector3 operator*(const Vector3& vec, const Matrix4& mat);

