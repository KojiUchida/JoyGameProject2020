#include "Camera.h"
#include "Math/MathUtility.h"
#include "Def/Screen.h"

Camera::Camera() :
	m_zoom(1.0f) {
}

Camera::~Camera() {
}

Camera& Camera::Instance() {
	static Camera instance;
	return instance;
}

void Camera::SetPosition(const Vector3& position) {
	m_position = position;
}

Vector3 Camera::GetPosition() {
	return m_position;
}

void Camera::SetRotation(const Vector3& rotation) {
	m_rotation = rotation;
}

Vector3 Camera::GetRotation() {
	return m_rotation;
}

void Camera::SetZoom(const float zoom) {
	m_zoom = zoom;
}

float Camera::GetZoom() {
	return m_zoom;
}

Matrix4 Camera::GetViewMatrix() {
	CalcViewMatrix();
	return m_viewMatrix;
}

Matrix4 Camera::GetProjectionMatrix() {
	CalcProjectionMatrix();
	return m_projectionMatrix;
}

Matrix4 Camera::GetOrthoMatrix() {
	CalcOrthoMatrix();
	return m_orthoMatrix;
}

Matrix4 Camera::GetRotationMatrix() {
	return m_rotationMatrix;
}

void Camera::CalcViewMatrix() {
	Vector3 up(0, 1, 0);
	Vector3 eye(m_position);
	Vector3 target(0, 0, 1);

	m_rotationMatrix = Matrix4::RotateRollPitchYaw(m_rotation);

	target = target * m_rotationMatrix;
	up = up * m_rotationMatrix;
	target = eye + target;

	m_viewMatrix = Matrix4::LookAt(eye, target, up);
}

void Camera::CalcProjectionMatrix() {
	m_projectionMatrix = Matrix4::Perspective(45, Screen::ASPECT, 0.1f, 1000.0f);
}

void Camera::CalcOrthoMatrix() {
	auto  trans = Matrix4::Translate(Vector3::Zero());
	auto  rot = Matrix4::Translate(Vector3(-Screen::WIDTH / 2, -Screen::HEIGHT / 2, 0)) *
		Matrix4::RotateZ(m_rotation.z) *
		Matrix4::Translate(Vector3(Screen::WIDTH / 2, Screen::HEIGHT / 2, 0));
	auto scale = Matrix4::Translate(Vector3(-Screen::WIDTH / 2, -Screen::HEIGHT / 2, 0)) *
		Matrix4::Scale(m_zoom) *
		Matrix4::Translate(Vector3(Screen::WIDTH / 2, Screen::HEIGHT / 2, 0));
	auto ortho = Matrix4::Identity();
	ortho.m[0][0] = 2.0f / Screen::WIDTH;
	ortho.m[1][1] = -2.0f / Screen::HEIGHT;
	ortho.m[3][0] = -1.0f;
	ortho.m[3][1] = 1.0f;

	m_orthoMatrix = trans * rot * scale * ortho;
}
