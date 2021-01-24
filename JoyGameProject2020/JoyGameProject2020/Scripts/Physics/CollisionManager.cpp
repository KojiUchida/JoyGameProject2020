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
	//for (auto& c1 : m_colliders) {
	//	for (auto& c2 : m_colliders) {
	//		if (c1 == c2)continue;

	//		c1->m_isCollide = c1->CheckCollision(c2);
	//		if (c1->m_isCollide && !c1->m_preCollide) {
	//			c1->gameObject->OnCollisionEnter(c2->gameObject);
	//			for (auto& c : c1->gameObject->GetComponents())
	//				c->OnCollisionEnter(c2->gameObject);
	//		}
	//		if (c1->m_isCollide && c1->m_preCollide) {
	//			c1->gameObject->OnCollisionStay(c2->gameObject);
	//			for (auto& c : c1->gameObject->GetComponents())
	//				c->OnCollisionStay(c2->gameObject);
	//		}
	//		if (!c1->m_isCollide && !c1->m_preCollide) {
	//			c1->gameObject->OnCollisionExit(c2->gameObject);
	//			for (auto& c : c1->gameObject->GetComponents())
	//				c->OnCollisionExit(c2->gameObject);
	//		}
	//	}
	//}

	m_colliders.clear();
}

void CollisionManager::Add(Collider3D* collider) {
	m_colliders.push_back(collider);
}
