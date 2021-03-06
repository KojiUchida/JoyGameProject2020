#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

class Camera {

private:
	Camera();
	~Camera();
	Camera(const Camera&) = delete;
	void operator=(const Camera&) = delete;

public:
	static Camera& Instance();

	/* 位置の設定 */
	void SetPosition(const Vector3& position);
	/* 位置の取得 */
	Vector3 GetPosition();

	/* 回転の設定 */
	void SetRotation(const Vector3& rotation);
	/* 回転の取得 */
	Vector3 GetRotation();

	void SetTarget(const Vector3& target);
	void UnlockTarget();

	/* MOE ビルボード行列取得 */
	Matrix4 GetBillboard();

	/* zoom値の設定 */
	void SetZoom(const float zoom);
	/* zoom値の取得 */
	float GetZoom();

	/* ビュー行列の取得 */
	Matrix4 GetViewMatrix();
	/* プロジェクション行列の取得 */
	Matrix4 GetProjectionMatrix();
	/* 平行投影行列の取得 */
	Matrix4 GetOrthoMatrix();

private:
	/* ビュー行列の計算 */
	void CalcViewMatrix();
	/* プロジェクション行列の計算 */
	void CalcProjectionMatrix();
	/* 平行投影行列の計算 */
	void CalcOrthoMatrix();

public:
	Quaternion rotation;

private:
	Vector3 m_position;
	Vector3 m_target;
	bool isTargetLock;
	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_orthoMatrix;
	float m_zoom;
};

