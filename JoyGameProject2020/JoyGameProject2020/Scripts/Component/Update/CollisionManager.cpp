#include "CollisionManager.h"
#include "Collider3D.h"
#include "GameObject/GameObject.h"

CollisionManager::CollisionManager() {
}

CollisionManager::~CollisionManager() {
}

CollisionManager& CollisionManager::Instance() {
	static CollisionManager instance;
	return instance;
}

void CollisionManager::Update() {
	for (auto& c1 : m_colliders) {
		for (auto& c2 : m_colliders) {
			if (c1 == c2)continue;

			c1->m_isCollide = c1->CheckCollision(c2.get());
			if (c1->m_isCollide && !c1->m_preCollide) {
				c1->gameObject.lock()->OnCollisionEnter(c2->gameObject.lock());
				for (auto& c : c1->gameObject.lock()->GetComponents())
					c->OnCollisionEnter(c2->gameObject.lock());
			}
			if (c1->m_isCollide && c1->m_preCollide) {
				c1->gameObject.lock()->OnCollisionStay(c2->gameObject.lock());
				for (auto& c : c1->gameObject.lock()->GetComponents())
					c->OnCollisionStay(c2->gameObject.lock());
			}
			if (!c1->m_isCollide && !c1->m_preCollide) {
				c1->gameObject.lock()->OnCollisionExit(c2->gameObject.lock());
				for (auto& c : c1->gameObject.lock()->GetComponents())
					c->OnCollisionExit(c2->gameObject.lock());
			}
		}
	}
}

void CollisionManager::Add(std::shared_ptr<Collider3D> collider) {
	m_colliders.push_back(collider);
}
