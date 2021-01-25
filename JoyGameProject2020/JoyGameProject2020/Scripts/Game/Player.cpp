#include "Player.h"
#include "Graphics/Model.h"
#include "Math/Matrix4.h"
#include "Math/MathUtil.h"
#include "Device/GameTime.h"
#include "Device/Input.h"
#include "Utility/Timer.h"
#include "Physics/CircleCollider3D.h"
#include <iostream>

Player::Player() :
	gauge(0.0f),
	currentSpeed(0.0f) {
}

Player::~Player() {
}

void Player::Init() {
	SetTag("Player");
	AddComponent(std::make_shared<Model>("sphere"));
	AddComponent(std::make_shared<CircleCollider3D>());
	SetScale(Vector3(2,4,2));

	attackTimer = std::make_unique<Timer>(1.0f);
	attackTimer->Start();

	stunTimer = std::make_unique<Timer>(0.5f);
	stunTimer->Start();
}

void Player::Update() {
	Rotation();
	Manipulation();
	gauge = Math::Clamp(gauge, 0.0f, 100.0f);
	Move();
	attackTimer->Update();
	stunTimer->Update();

	//std::cout << gauge << std::endl;
}

void Player::Shutdown() {
}

void Player::OnCollisionEnter(std::shared_ptr<GameObject> other) {
	if (other->CompareTag("Obstacle")) {
		gauge -= 10;
	}
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
	rot.z += Input::Gyro().z * ROT_SPEED;
	SetRotation(rot);

	up = Vector3(0, 1, 0);
	up = (up * Matrix4::RotationFromQuaternion(transform.rotation)).Normalize();
}

void Player::Charge() {
	gauge += Input::Accel().Length() * GAUGE_INC_RATIO * 300.0f;
}

void Player::Attack() {
	if (!CanAttack()) return;
	attackTimer->Reset();
	currentSpeed = GaugeRatio() * ATTACK_SPEED * GameTime::DeltaTime();
	velocity = up * currentSpeed;
	gauge -= ATTACK_GAUGE;
}

void Player::Fly() {
	gauge -= GAUGE_DEC_RATIO;
	if (!CanAttack()) return;
	float speed = GaugeRatio() * MOVE_SPEED * GameTime::DeltaTime();
	speed = Math::Clamp(speed, 0.0f, currentSpeed);
	velocity = up * speed;
}

void Player::Move() {
	accel = 0;
	accel = accel * up;
	accel.y -= GRAVITY;
	velocity += accel * GameTime::DeltaTime();
	auto resist = velocity.x / abs(velocity.x) * AIR_RESIST * GameTime::DeltaTime();
	resist = velocity.x < 0.001f ? 0 : resist;
	velocity.x -= resist;

	auto pos = GetPosition();
	pos += velocity;
	pos.x = Math::Clamp(pos.x, -65.0f, 65.0f);
	pos.y = Math::Clamp(pos.y, 0.0f, 1000.0f);
	SetPosition(pos);
}

float Player::GaugeRatio() {
	return (gauge / GAUGE_MAX);
}

bool Player::CanAttack() {
	return gauge > ATTACK_GAUGE;
}

float Player::Gauge() {
	return gauge;
}
