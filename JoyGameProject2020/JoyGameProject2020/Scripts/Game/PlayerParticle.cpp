#include "PlayerParticle.h"
#include "Utility/Random.h"
#include "Graphics/Model.h"

PlayerParticle::PlayerParticle(const Vector3& size) :
	m_maxSize(size) {
}

PlayerParticle::~PlayerParticle() {
}

void PlayerParticle::Init() {
	m_destroyTimer = std::make_unique<Timer>(DEATH_TIME);
	m_destroyTimer->Start();

	Random rnd;
	SetRotation(Vector3(
		rnd.GetRand(0.0f, 360.0f),
		rnd.GetRand(0.0f, 360.0f),
		rnd.GetRand(0.0f, 360.0f)
	));
	SetScale(Vector3(MAX_SIZE));


	m_model = std::make_shared<Model>("cube");
	m_color = Color4(
		rnd.GetRand(0.5f, 0.8f),
		rnd.GetRand(0.5f, 0.8f),
		rnd.GetRand(0.5f, 0.8f),
		1.0f
	);
	m_model->SetColor(m_color);
	AddComponent(m_model);

}

void PlayerParticle::Update() {
	m_destroyTimer->Update();
	SetScale(Vector3(m_maxSize * m_destroyTimer->GetRatioRemaining()));
	//SetPosition(GetPosition() + m_velocity);
	//m_velocity = Vector3::Lerp(m_velocity, Vector3::Zero(), 0.1f);
	m_color.a = m_destroyTimer->GetRatioRemaining();
	m_model->SetColor(m_color);

	if (m_destroyTimer->IsTime()) {
		Destroy();
	}
}
