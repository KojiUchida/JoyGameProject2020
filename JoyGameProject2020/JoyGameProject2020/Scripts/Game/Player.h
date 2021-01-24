#pragma once
#include <memory>
#include "GameObject/GameObject.h"
#include "Math/Vector3.h"

class Timer;

namespace {
	const float ROT_SPEED = 30.0f;
	const float GAUGE_INC_RATIO = 0.002f;
	const float GAUGE_DEC_RATIO = 0.05f;
	const float GAUGE_MAX = 100.0f;
	const float ATTACK_GAUGE = 10.0f;
	const float MOVE_SPEED = 100.0f;
	const float ATTACK_SPEED = 100.0f;
	const float GRAVITY = 0.1f;
	const float AIR_RESIST = 0.1f;
}

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
	void Fly();
	void Move();
	float GaugeRatio();
	bool CanAttack();

	float Gauge();

private:
	float gauge;
	float currentSpeed;

	Vector3 velocity;
	Vector3 accel;
	Vector3 up;

	std::unique_ptr<Timer> attackTimer;
	std::unique_ptr<Timer> stunTimer;
};

