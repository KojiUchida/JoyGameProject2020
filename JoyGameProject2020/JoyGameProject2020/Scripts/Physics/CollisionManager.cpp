#include "CollisionManager.h"
#include "Collider3D.h"
#include "GameObject/GameObject.h"
#include <omp.h>
#include <functional>

CollisionManager::CollisionManager() {
}

CollisionManager::~CollisionManager() {
}

CollisionManager& CollisionManager::Instance() {
	static CollisionManager instance;
	return instance;
}

void CollisionManager::Update() {
	auto size = m_colliders.size();

#pragma omp parallel for
	for (int i = 0; i < size;++i) {
		for (int j = 0; j < size;++j) {
			auto c1 = m_colliders[i];
			auto c2 = m_colliders[j];

			if (c1 == c2)continue;

			/* すっげースマートじゃない気がする */
			if (c1->CheckCollision(c2)) {
				c1->Add(c2);
				if (!c1->IsPreCollide(c2)) {
					c1->gameObject.lock()->OnCollisionEnter(c2->gameObject.lock());
					for (auto c : c1->gameObject.lock()->GetComponents()) {
						c->OnCollisionEnter(c2->gameObject.lock());
					}
				} else {
					c1->gameObject.lock()->OnCollisionStay(c2->gameObject.lock());
					for (auto c : c1->gameObject.lock()->GetComponents()) {
						c->OnCollisionStay(c2->gameObject.lock());
					}
				}
			} else {
				if (c1->IsPreCollide(c2)) {
					c1->gameObject.lock()->OnCollisionExit(c2->gameObject.lock());
					for (auto c : c1->gameObject.lock()->GetComponents()) {
						c->OnCollisionExit(c2->gameObject.lock());
					}
				}
			}
		}
	}

	m_colliders.clear();
}

void CollisionManager::Add(Collider3D* collider) {
	m_colliders.push_back(collider);
}
