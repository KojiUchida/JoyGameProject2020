#include "Goal.h"
#include "GameManager.h"
#include "Physics/BoxCollider3D.h"

void Goal::Init() {
	SetTag("Goal");
	AddComponent(std::make_shared<BoxCollider3D>());
}

void Goal::OnCollisionEnter(std::shared_ptr<GameObject> other) {
	if (other->CompareTag("Player")) {
		GameManager::Instance().ChangeState(GameState::GOAL);
	}
}
