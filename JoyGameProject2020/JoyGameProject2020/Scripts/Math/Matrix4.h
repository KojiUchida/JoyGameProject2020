#pragma once
#include "Quaternion.h"

struct Vector3;
struct Quaternion;
struct Matrix4 {

	typedef float Degree;
	typedef float Radian;

	float m[4][4];

	/* �R���X�g���N�^ */
	Matrix4();
	/* �R���X�g���N�^ */
	Matrix4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	/* ������Z�q */
	Matrix4& operator=(const Matrix4& mat);

	/* �P�ʍs�� */
	static const Matrix4 Identity();
	/* �g��E�k���s�� */
	static const Matrix4 Scale(const Vector3& vec);
	/* ���s�ړ��s�� */
	static const Matrix4 Translate(const Vector3& vec);

	static const Matrix4 RotationFromQuaternion(const Quaternion& q);
	/* X����]�s�� */
	static const Matrix4 Rotation(const Vector3& x, const Vector3& y, const Vector3& z);
	/* X����]�s�� */
	static const Matrix4 RotateX(const Radian rot);
	/* Y����]�s�� */
	static const Matrix4 RotateY(const Radian rot);
	/* Z����]�s�� */
	static const Matrix4 RotateZ(const Radian rot);
	/* RollPitchYaw�p�ɂ���]�s�� */
	static const Matrix4 RotateRollPitchYaw(const Vector3& vec);
	static const Matrix4 RotateRollPitchYaw(const Radian pitch, const  Radian yaw, const  Radian roll);

	/* �����ϊ��s�� */
	static const Matrix4 Perspective(
		const Degree fov, const float aspect, const float near, const float far);
	/* ����ϊ��s�� */
	static const Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/* �]�u�s�� */
	Matrix4 transpose() const;
	/* �t�s�� */
	Matrix4 Inverse() const;
	/* ���W�ϊ�(�x�N�g���ƍs��̊|���Z) */
	Vector3 transform(const Vector3& vec, const Matrix4& mat) const;
	/* ���W�ϊ�(�@���x�N�g���p�A�ړ��͍s��Ȃ�) */
	Vector3 transformNormal(const Vector3& vec, const Matrix4& mat) const;
};

/* �񍀉��Z�q */
const Matrix4 operator+(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator-(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat1, const Matrix4& mat2);
const Matrix4 operator*(const Matrix4& mat, const float scalar);
const Matrix4 operator*(const float scalar, const Matrix4& mat);
const Vector3 operator*(const Vector3& vec, const Matrix4& mat);

