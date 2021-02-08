#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4.h"

class Camera {

private:
	Camera();
	~Camera();
	Camera(const Camera&) = delete;
	void operator=(const Camera&) = delete;

public:
	static Camera& Instance();

	/* �ʒu�̐ݒ� */
	void SetPosition(const Vector3& position);
	/* �ʒu�̎擾 */
	Vector3 GetPosition();

	/* ��]�̐ݒ� */
	void SetRotation(const Vector3& rotation);
	/* ��]�̎擾 */
	Vector3 GetRotation();

	void SetTarget(Vector3* target);

	/* MOE �r���{�[�h�s��擾 */
	Matrix4 GetBillboard();

	/* zoom�l�̐ݒ� */
	void SetZoom(const float zoom);
	/* zoom�l�̎擾 */
	float GetZoom();

	/* �r���[�s��̎擾 */
	Matrix4 GetViewMatrix();
	/* �v���W�F�N�V�����s��̎擾 */
	Matrix4 GetProjectionMatrix();
	/* ���s���e�s��̎擾 */
	Matrix4 GetOrthoMatrix();
	/* ��]�s��̎擾 */
	Matrix4 GetRotationMatrix();

private:
	/* �r���[�s��̌v�Z */
	void CalcViewMatrix();
	/* �v���W�F�N�V�����s��̌v�Z */
	void CalcProjectionMatrix();
	/* ���s���e�s��̌v�Z */
	void CalcOrthoMatrix();

private:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3* m_target;
	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_orthoMatrix;
	Matrix4 m_rotationMatrix;
	float m_zoom;
};

