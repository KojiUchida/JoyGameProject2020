#include "Player.h"
#include "Component/Draw/ObjRenderer.h"
#include "Math/Matrix4.h"
#include "Math/MathUtility.h"
#include "Device/GameTime.h"
#include "Device/Input.h"
#include "Utility/Timer.h"
#include <iostream>

Player::Player() :
	rotSpeed(30.0f),
	gauge(0.0f),
	gaugeMax(100.0f),
	gaugeIncRatio(0.002f),
	gaugeDecRatio(0.05f),
	attackGauge(10.0f),
	currentSpeed(0.0f),
	flySpeed(100.0f),
	attackSpeed(150.0f),
	grav(0.1f),
	airResist(0.01f) {
}

Player::~Player() {
}

void Player::Init() {
	AddComponent(std::make_shared<ObjRenderer>("dosei"));
	SetScale(Vector3(0.05f));

	attackTimer = std::make_unique<Timer>(1.0f);
	attackTimer->Start();

	stunTimer = std::make_unique<Timer>(0.5f);
	stunTimer->Start();
}

void Player::Update() {
	Rotation();
	Manipulation();
	gauge = MathUtility::Clamp(gauge, 0, 100);
	Move();
	attackTimer->Update();
	stunTimer->Update();

	std::cout << gauge << std::endl;
}

void Player::Shutdown() {
}

void Player::Manipulation() {
	if (!stunTimer->IsTime()) return;

	if (Input::IsButton(PadButton::R1)) {
		Charge();
	} else {
		Fly();
	}

	if (Input::IsButtonUp(PadButton::R1)) {
		Attack();
	}
}

void Player::Rotation() {
	auto rot = GetRotation();
	rot.z += Input::Gyro().z * rotSpeed * GameTime::DeltaTime();
	SetRotation(rot);

	up = Vector3(0, 1, 0);
	up = (up * Matrix4::RotateRollPitchYaw(rot.x, -rot.y, rot.z)).Normalize();
}

void Player::Charge() {
	gauge += Input::Accel().Length() * gaugeIncRatio;
}

void Player::Attack() {
	if (!CanAttack()) return;
	attackTimer->Reset();
	currentSpeed = GaugeRatio() * attackSpeed * GameTime::DeltaTime();
	velocity = up * currentSpeed;
	gauge -= attackGauge;
}

void Player::Fly() {
	gauge -= gaugeDecRatio;
	if (!CanAttack()) return;
	auto speed = GaugeRatio() * flySpeed * GameTime::DeltaTime();
	speed = MathUtility::Clamp(speed, 0, currentSpeed);
	velocity = up * speed;
}

void Player::Move() {
	accel = 0;
	accel = accel * up;
	accel.y -= grav;
	velocity += accel * GameTime::DeltaTime();
	auto resist = velocity.x / abs(velocity.x) * airResist * GameTime::DeltaTime();
	resist = velocity.x < 0.001f ? 0 : resist;
	velocity.x -= resist;

	auto pos = GetPosition();
	pos += velocity;
	pos.x = MathUtility::Clamp(pos.x, -65, 65);
	pos.y = MathUtility::Clamp(pos.y, 0, 1000);
	SetPosition(pos);
}

float Player::GaugeRatio() {
	return (gauge / gaugeMax);
}

bool Player::CanAttack() {
	return gauge > attackGauge;
}

float Player::Gauge() {
	return gauge;
}
