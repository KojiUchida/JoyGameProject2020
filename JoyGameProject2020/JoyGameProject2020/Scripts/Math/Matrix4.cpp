#include "Matrix4.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector3.h"

Matrix4::Matrix4() :
	m{ 0 } {
}

Matrix4::Matrix4(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44) :
	m{ m11, m12,m13, m14,
	m21, m22, m23, m24,
	m31, m32, m33, m34,
	m41, m42, m43, m44, } {
}

Matrix4& Matrix4::operator=(const Matrix4& mat) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			m[row][col] = mat.m[row][col];
		}
	}
	return *this;
}

const Matrix4 Matrix4::Identity() {
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::Scale(const Vector3& vec) {
	return Matrix4(
		vec.x, 0, 0, 0,
		0, vec.y, 0, 0,
		0, 0, vec.z, 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::Translate(const Vector3& vec) {
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		vec.x, vec.y, vec.z, 1
	);
}

const Matrix4 Matrix4::Rotation(const Vector3& x, const Vector3& y, const Vector3& z) {
	return Matrix4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::RotateX(const float angle) {
	float rad = angle * (float)M_PI / 180.0f;
	return Matrix4(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::RotateY(const float angle) {
	float rad = angle * (float)M_PI / 180.0f;
	return Matrix4(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::RotateZ(const float angle) {
	float rad = angle * (float)M_PI / 180.0f;
	return Matrix4(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

const Matrix4 Matrix4::RotateRollPitchYaw(const Vector3& vec) {
	return Matrix4::RotateZ(vec.z) * Matrix4::RotateX(vec.x) * Matrix4::RotateY(vec.y);
}

const Matrix4 Matrix4::RotateRollPitchYaw(const float pitch, const float yaw, const float roll) {
	return Matrix4::RotateZ(roll) * Matrix4::RotateX(pitch) * Matrix4::RotateY(yaw);
}

const Matrix4 Matrix4::Perspective(const float fov, const float aspect, const float near, const float far) {
	auto radfov = fov * (float)M_PI / 180.0f;
	float height = 1.0f / tanf(radfov / 2.0f);
	float width = height / aspect;
	return Matrix4(
		width, 0, 0, 0,
		0, height, 0, 0,
		0, 0, far / (far - near), 1,
		0, 0, -far * near / (far - near), 0
	);
}

const Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
	Vector3 z = (target - eye).Normalize();
	Vector3 x = Vector3::Cross(up, z).Normalize();
	Vector3 y = Vector3::Cross(z, x).Normalize();

	return Matrix4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-Vector3::Dot(x, eye), -Vector3::Dot(y, eye), -Vector3::Dot(z, eye), 1
	);
}

Matrix4 Matrix4::transpose() const {
	double Result[4][4];
	double tmp[12];
	double src[16];
	double det;

	for (unsigned int i = 0; i < 4; i++) {
		src[i + 0] = (*this).m[i][0];
		src[i + 4] = (*this).m[i][1];
		src[i + 8] = (*this).m[i][2];
		src[i + 12] = (*this).m[i][3];
	}

	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	Result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	Result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	Result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	Result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	Result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	Result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	Result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	Result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];

	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	Result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	Result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	Result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	Result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	Result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	Result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	Result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	Result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	Result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	Result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	Result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	Result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	Result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	Result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	Result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	Result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	det = src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3];

	det = 1.0f / det;

	Matrix4 FloatResult;
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			FloatResult.m[i][j] = float(Result[i][j] * det);
		}
	}
	return FloatResult;
}

Matrix4 Matrix4::Inverse() const {
	return Matrix4();
}

Vector3 Matrix4::transform(const Vector3& vec, const Matrix4& mat) const {
	float w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + mat.m[3][3];
	Vector3 result = {
		(vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0]) / w,
		(vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1]) / w,
		(vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2]) / w
	};
	return result;
}

Vector3 Matrix4::transformNormal(const Vector3& vec, const Matrix4& mat) const {
	Vector3 result = {
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2]
	};
	return result;
}

const Matrix4 operator+(const Matrix4& mat1, const Matrix4& mat2) {
	Matrix4 result;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = mat1.m[row][col] + mat2.m[row][col];
		}
	}
	return result;
}

const Matrix4 operator-(const Matrix4& mat1, const Matrix4& mat2) {
	Matrix4 result;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = mat1.m[row][col] - mat2.m[row][col];
		}
	}
	return result;
}

const Matrix4 operator*(const Matrix4& mat1, const Matrix4& mat2) {
	Matrix4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
			}
		}
	}
	return result;
}

const Matrix4 operator*(const Matrix4& mat, const float scalar) {
	Matrix4 result;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = mat.m[row][col] * scalar;
		}
	}
	return result;
}

const Matrix4 operator*(const float salar, const Matrix4& mat) {
	Matrix4 result;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = mat.m[row][col] * salar;
		}
	}
	return result;
}

const Vector3 operator*(const Vector3& vec, const Matrix4& mat) {
	Vector3 result;

	result.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0];
	result.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1];
	result.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2];

	return result;
}

