#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component/Component.h"

GameObjectManager::GameObjectManager() {
}

GameObjectManager::~GameObjectManager() {
}

void GameObjectManager::Update() {
	for (const auto& o : m_gameObjects) {
		o->Update();

		for (const auto& c : o->m_components) {
			c->Update();
		}

		for (const auto& c : o->m_children) {
			c->Update();
		}
	}

	for (unsigned int i = 0; i < m_gameObjects.size();) {
		if (m_gameObjects[i]->m_isDestroy) {

			m_gameObjects[i]->Shutdown();

			for (auto& c : m_gameObjects[i]->m_components) {
				c->Shutdown();
			}

			for (auto& c : m_gameObjects[i]->m_children) {
				c->Shutdown();
			}

			/* erase‚æ‚è‚à‘‚¢‚Ì‚ÅÌ—p */
			m_gameObjects[i] = m_gameObjects.back();
			m_gameObjects.pop_back();
			continue;
		}
		++i;
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
