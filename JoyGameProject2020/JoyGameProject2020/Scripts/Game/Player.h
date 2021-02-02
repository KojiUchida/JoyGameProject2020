#pragma once
#include <memory>
#include "GameObject/GameObject.h"
#include "Math/Vector3.h"

namespace {
	const float ROT_SPEED			=	300.0f;		/* ��]�̑��x */
	const float GAUGE_INC_RATIO		=	0.01f;		/* �Q�[�W�̃`���[�W�̏㏸�� */
	const float GAUGE_DEC_RATIO		=	0.05f;		/* �Q�[�W�̌����� */
	const float GAUGE_MAX			=	100.0f;		/* �Q�[�W�̍ő�l */
	const float ATTACK_GAUGE		=	10.0f;		/* �U���Ɏg���Q�[�W�� */
	const float MOVE_SPEED			=	40.0f;		/* �ʏ�ړ����x */
	const float ATTACK_SPEED		=	150.0f;		/* �U�����̑��x */
	const float GRAVITY				=	0.5f;		/* �d�� */
	const float AIR_RESIST			=	0.1f;		/* ��C��R */
	const float SPEED_DEC_RATIO		=	0.02f;		/* �ʏ�ړ��A�U������`���[�W�ɂ����Ă̑��x������ */
	const float ATTACK_TIME			=	0.3f;		/* �U���p������ */
	const float FLY_DEC_RATIO		=	0.1f;		/* �U������ʏ�ړ��ɂ����Ă̑��x������ */
	const float FALL_SPEED_MAX		=	50.0f;		/* �ő嗎�����x */
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

