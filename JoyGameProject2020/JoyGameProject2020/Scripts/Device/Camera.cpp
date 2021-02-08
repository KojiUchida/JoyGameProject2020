#include "Camera.h"
#include "Math/MathUtil.h"
#include "Def/Screen.h"

Camera::Camera() :
	m_zoom(1.0f),
	m_target(nullptr)
{
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

void Camera::SetTarget(Vector3 * target)
{
	m_target = target;
}

Matrix4 Camera::GetBillboard()
{
	//視点座標
	Vector3 eyePosition = m_position;

	Vector3 target(0, 0, 1);

	target = target * m_rotationMatrix;
	target = m_position + target;
	//注視点座標
	Vector3 targetPosition = target;
	//上方向
	Vector3 upVector = Vector3(0, 1, 0);

	//*************************************************************************

		//カメラZ軸(視線方向）
	Vector3 cameraAxisZ = targetPosition - eyePosition;

	//0ベクトルだと向きが定まらないので除外
	//_ASSERT_EXPR(cameraAxisZ != Vector3(0), L"");
	//assert(!XMVector3IsInfinite(cameraAxisZ));
	//_ASSERT_EXPR(upVector != Vector3(0), L"");
	//assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = cameraAxisZ.Normalize();

	//*************************************************************************

	//*************************************************************************

		//カメラX軸(右方向)
	Vector3 cameraAxisX;
	//x軸は上方向→Z軸の外積で求まる
	cameraAxisX = Vector3::Cross(upVector, cameraAxisZ);
	//正規化
	cameraAxisX = cameraAxisX.Normalize();

	//*************************************************************************

	//*************************************************************************

	//カメラY軸(右方向)
	Vector3 cameraAxisY;
	//y軸はZ軸→X軸の外積で求まる
	cameraAxisY = Vector3::Cross(cameraAxisZ, cameraAxisX);
	//正規化
	cameraAxisY = cameraAxisY.Normalize();


	//*************************************************************************

	//カメラ回転行列
	//カメラ座標系→ワールド座標系の変換行列
	Matrix4 matCameraRot = Matrix4(
		cameraAxisX.x, cameraAxisX.y, cameraAxisX.z, 0,
		cameraAxisY.x, cameraAxisY.y, cameraAxisY.z, 0,
		cameraAxisZ.x, cameraAxisZ.y, cameraAxisZ.z, 0,
		0, 0, 0, 1
	);

	return matCameraRot;
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

	if (m_target == nullptr)
	{
		m_rotationMatrix = Matrix4::RotateRollPitchYaw(m_rotation);

		Vector3 target = target * m_rotationMatrix;
		up = up * m_rotationMatrix;
		target = eye + target;
		m_viewMatrix = Matrix4::LookAt(eye, target, up);
		return;
	}


	m_viewMatrix = Matrix4::LookAt(eye, *m_target, up);
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
