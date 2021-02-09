#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameObject/Component.h"
#include <algorithm>

GameObjectManager::GameObjectManager() {
}

GameObjectManager::~GameObjectManager() {
}

GameObjectManager& GameObjectManager::Instance() {
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::Update() {
	int objCounut = m_gameObjects.size();
	for (unsigned int i = 0; i < objCounut;) {
		if (m_gameObjects[i]->m_isDestroy) {

			m_gameObjects[i]->Shutdown();

			for (auto& c : m_gameObjects[i]->m_components) {
				c->Shutdown();
			}

			for (auto& c : m_gameObjects[i]->m_children) {
				c->Shutdown();
			}

			/* eraseÇÊÇËÇ‡ëÅÇ¢ÇÃÇ≈çÃóp */
			m_gameObjects[i] = m_gameObjects.back();
			m_gameObjects.pop_back();
			objCounut--;
			continue;
		}
		++i;
	}

	for (unsigned int i = 0; i < objCounut; ++i) {
		auto o = m_gameObjects[i];
		o->Update();

		for (auto c : o->m_components) {
			c->Update();
		}

		for (auto c : o->m_children) {
			c->Update();
		}
	}
}

void GameObjectManager::Shutdown() {
	for (const auto& o : m_gameObjects) {
		o->Shutdown();

		for (const auto& c : o->m_components) {
			c->Shutdown();
		}

		for (const auto& c : o->m_children) {
			c->Shutdown();
		}
	}
}

void GameObjectManager::Add(const std::shared_ptr<GameObject> gameObject) {
	gameObject->Init();

	for (const auto& c : gameObject->m_components) {
		c->Init();
	}

	for (const auto& c : gameObject->m_children) {
		c->Init();
	}

	m_gameObjects.push_back(gameObject);
}

void GameObjectManager::Clear() {
	m_gameObjects.clear();
}
