#include "Player.h"
#include "Component/Draw/ObjRenderer.h"
#include "Math/Matrix4.h"
#include "Math/MathUtility.h"
#include "Device/GameTime.h"
#include "Device/Input.h"
#include <iostream>

Player::Player() :
	gauge(0),
	rotSpeed(30),
	speed(100), 
	grav(0.1f){
}

Player::~Player() {
}

void Player::Init() {
	AddComponent(std::make_shared<ObjRenderer>("dosei"));
	SetScale(Vector3(0.05f));
	/*AddComponent(std::make_shared<ObjRenderer>("sphere"));
	SetScale(Vector3(1, 2, 1));*/
}

void Player::Update() {
	accel = 0;

	auto rot = GetRotation();
	rot.z += Input::Gyro().z * rotSpeed * GameTime::DeltaTime();
	SetRotation(rot);

	Vector3 up = Vector3(0, 1, 0);
	up = (up * Matrix4::RotateRollPitchYaw(rot.x, -rot.y, rot.z)).Normalize();

	gauge-= 0.05f;
	if (Input::IsButton(PadButton::R1)) {
		gauge += 0.1f;
	}
	if (Input::IsButtonUp(PadButton::R1)) {
		velocity = up * gauge * 0.003f;
	}
	gauge = MathUtility::Clamp(gauge, 0, 100);
	std::cout << gauge << std::endl;

	//auto min = Input::IsButton(PadButton::R1) ? 0 : speed;
	//auto max = speed * 1.5f;
	//auto currentSpeed = MathUtility::Clamp(speed, min, max);

	accel = accel * up;
	accel.y -= grav;
	velocity += accel * GameTime::DeltaTime();

	auto pos = GetPosition();
	pos += velocity;
	pos.x = MathUtility::Clamp(pos.x, -65, 65);
	pos.y = MathUtility::Clamp(pos.y, 0, 1000);
	SetPosition(pos);
}

void Player::Shutdown() {
}
