#pragma once
#include <memory>
#include "GameObject/GameObject.h"
#include "Utility/Timer.h"
#include "Utility/Color4.h"

namespace {
	const float MAX_SIZE = 0.5f;
	const float DEATH_TIME = 0.5f;
}

class Model;
class PlayerParticle : public GameObject {
public:
	PlayerParticle(const Vector3& size = MAX_SIZE);
	~PlayerParticle();

	virtual void Init() override;
	virtual void Update() override;
private:
	std::unique_ptr<Timer> m_destroyTimer;
	std::shared_ptr<Model> m_model;
	Vector3 m_maxSize;
	Color4 m_color;
};

