#pragma once
#include <memory>
#include "GameObject/GameObject.h"
#include "Math/Vector3.h"

namespace {
	const float ROT_SPEED			=	300.0f;		/* 回転の速度 */
	const float GAUGE_INC_RATIO		=	0.01f;		/* ゲージのチャージの上昇率 */
	const float GAUGE_DEC_RATIO		=	0.05f;		/* ゲージの減少率 */
	const float GAUGE_MAX			=	100.0f;		/* ゲージの最大値 */
	const float ATTACK_GAUGE		=	10.0f;		/* 攻撃に使うゲージ量 */
	const float MOVE_SPEED			=	40.0f;		/* 通常移動速度 */
	const float ATTACK_SPEED		=	150.0f;		/* 攻撃時の速度 */
	const float GRAVITY				=	0.5f;		/* 重力 */
	const float AIR_RESIST			=	0.1f;		/* 空気抵抗 */
	const float SPEED_DEC_RATIO		=	0.02f;		/* 通常移動、攻撃からチャージにかけての速度減少率 */
	const float ATTACK_TIME			=	0.3f;		/* 攻撃継続時間 */
	const float FLY_DEC_RATIO		=	0.1f;		/* 攻撃から通常移動にかけての速度減少率 */
	const float FALL_SPEED_MAX		=	50.0f;		/* 最大落下速度 */
}

class Timer;
class Model;
class GameObjectManager;
class Player : public GameObject {

public:
	Player();
	~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	virtual void OnCollisionEnter(std::shared_ptr<GameObject> other) override;

	void Manipulation();
	void Rotation();
	void Charge();
	void Attack();
	void Attacking();
	void Fly();
	void Neutral();
	void Move();

	bool CanAttack();
	bool IsAttacking();
	bool CanFly();

	float Gauge();
	float GaugeRatio();

private:
	GameObjectManager& m_objManager;

	float m_gauge;
	float m_currentSpeed;

	Vector3 m_velocity;
	Vector3 m_accel;

	std::unique_ptr<Timer> m_attackTimer;
	std::shared_ptr<Model> m_model;
};

