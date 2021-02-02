#include "Player.h"
#include <iostream>
#include "GameObject/GameObjectManager.h"
#include "Graphics/Model.h"
#include "Math/Matrix4.h"
#include "Math/MathUtil.h"
#include "Device/GameTime.h"
#include "Device/Input.h"
#include "Utility/Timer.h"
#include "Utility/Random.h"
#include "Physics/CircleCollider3D.h"
#include "Game/PlayerParticle.h"

Player::Player() :
	m_objManager(GameObjectManager::Instance()),
	m_gauge(0.0f),
	m_currentSpeed(0.0f) {
}

Player::~Player() {
}

void Player::Init() {
	SetTag("Player");
	m_model = std::make_shared<Model>("sphere");
	AddComponent(m_model);
	AddComponent(std::make_shared<CircleCollider3D>());
	SetScale(Vector3(2, 4, 2));

	m_attackTimer = std::make_unique<Timer>(ATTACK_TIME);
	m_attackTimer->Start();
}

void Player::Update() {
	Rotation();
	Manipulation();
	m_gauge = Math::Clamp(m_gauge, 0.0f, GAUGE_MAX);
	Move();
	m_attackTimer->Update();
}

void Player::Shutdown() {
}

void Player::OnCollisionEnter(std::shared_ptr<GameObject> other) {
	if (other->CompareTag("Obstacle")) {
		m_gauge -= 10;
	}
}

void Player::Manipulation() {
	if (Input::IsButton(PadButton::R1)) {
		Charge();
	} else if (IsAttacking()) {
		Attacking();
	} else if (CanFly()) {
		Fly();
	} else {
		Neutral();
	}

	if (Input::IsButtonUp(PadButton::R1)) {
		Attack();
	}
}

void Player::Rotation() {
	auto rot = GetRotation();
	rot.z += Input::Gyro().z * ROT_SPEED;
	SetRotation(rot);
}

void Player::Charge() {
	m_gauge += Input::Accel().Length() * GAUGE_INC_RATIO * 300.0f;
	m_velocity.x = Math::Lerp(m_velocity.x, 0.0f, SPEED_DEC_RATIO);
	if (m_velocity.y > 0) {
		m_velocity.y = Math::Lerp(m_velocity.y, 0.0f, SPEED_DEC_RATIO);
	}

	m_model->SetColor(Color4(1.0f, 1.0f, 0.1f, 1.0f));
}

void Player::Attack() {
	if (!CanAttack()) return;
	m_gauge -= ATTACK_GAUGE;
	m_attackTimer->Reset();

	Random rnd;
	auto offset = 10.0f;
	for (int i = 0; i < 20; ++i) {
		auto p = std::make_shared<PlayerParticle>(1);
		p->SetPosition(GetPosition() +
			Vector3(rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset)));
		m_objManager.Add(p);
	}
}

void Player::Attacking() {
	if (!CanFly())return;
	if (!IsAttacking())return;
	m_currentSpeed = ATTACK_SPEED * GameTime::DeltaTime();
	m_velocity = Up() * m_currentSpeed;

	Random rnd;
	auto offset = 0.2f;
	auto p = std::make_shared<PlayerParticle>();
	p->SetPosition(GetPosition() +
		Vector3(rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset)));
	m_objManager.Add(p);

	m_model->SetColor(Color4(1.0f, 0.1f, 0.1f, 1.0f));
}

void Player::Fly() {
	if (!CanFly()) return;
	if (IsAttacking())return;
	m_gauge -= GAUGE_DEC_RATIO;
	float speed = MOVE_SPEED * GameTime::DeltaTime();
	m_currentSpeed = Math::Lerp(m_currentSpeed, speed, FLY_DEC_RATIO);
	m_velocity = Up() * m_currentSpeed;

	Random rnd;
	auto offset = 0.2f;
	auto p = std::make_shared<PlayerParticle>();
	p->SetPosition(GetPosition() +
		Vector3(rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset), rnd.GetRand(-offset, offset)));
	m_objManager.Add(p);

	m_model->SetColor(Color4(0.1f, 1.0f, 1.0f, 1.0f));
}

void Player::Neutral() {
	m_velocity.x = Math::Lerp(m_velocity.x, 0.0f, SPEED_DEC_RATIO);
	if (m_velocity.y > 0) {
		m_velocity.y = Math::Lerp(m_velocity.y, 0.0f, SPEED_DEC_RATIO);
	}

	m_model->SetColor(Color4(1.0f, 1.0f, 1.0f, 1.0f));
	float min = static_cast<float>(-FALL_SPEED_MAX * GameTime::DeltaTime());
	m_velocity.y = Math::Clamp(m_velocity.y, min, 10000.0f);
}

void Player::Move() {
	m_accel = 0;
	m_accel.y -= GRAVITY;
	m_velocity += m_accel * GameTime::DeltaTime();

	//auto resist = m_velocity.x / abs(m_velocity.x) * AIR_RESIST * GameTime::DeltaTime();
	//resist = abs(m_velocity.x) < 0.001f ? 0 : resist;
	//m_velocity.x -= resist;

	auto pos = GetPosition();
	pos += m_velocity;
	pos.x = Math::Clamp(pos.x, -65.0f, 65.0f);
	pos.y = Math::Clamp(pos.y, 0.0f, 1000.0f);
	SetPosition(pos);
}

float Player::GaugeRatio() {
	return (m_gauge / GAUGE_MAX);
}

bool Player::CanAttack() {
	return m_gauge > ATTACK_GAUGE;
}

bool Player::IsAttacking() {
	return !m_attackTimer->IsTime();
}

bool Player::CanFly() {
	return m_gauge > 0.0f;
}

float Player::Gauge() {
	return m_gauge;
}
