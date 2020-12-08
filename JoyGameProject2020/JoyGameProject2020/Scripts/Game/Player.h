#pragma once
#include <memory>
#include "GameObject/GameObject.h"
#include "Math/Vector3.h"

class Timer;
class Player : public GameObject {

public:
	Player();
	~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;

	void Manipulation();
	void Rotation();
	void Charge();
	void Attack();
	void Fly();
	void Move();
	float GaugeRatio();
	bool CanAttack();

private:
	const float rotSpeed;

	float gauge;
	const float gaugeMax;
	const float attackGauge;

	float currentSpeed;
	const float flySpeed;
	const float attackSpeed;

	const float grav;
	const float airResist;

	Vector3 velocity;
	Vector3 accel;
	Vector3 up;

	std::unique_ptr<Timer> attackTimer;
	std::unique_ptr<Timer> stunTimer;
};

