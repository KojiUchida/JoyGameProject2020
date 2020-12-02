#pragma once
#include "GameObject/GameObject.h"
#include "Math/Vector3.h"

class Player : public GameObject {

	enum {
		Idle,
		Charge,
		Move,
	};

public:
	Player();
	~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;

private:
	float gauge;
	const float rotSpeed;
	const float speed;
	const float grav;
	Vector3 velocity;
	Vector3 accel;
};

