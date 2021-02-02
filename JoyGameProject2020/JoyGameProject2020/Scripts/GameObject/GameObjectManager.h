#pragma once
#include <vector>
#include <memory>

class GameObject;
class GameObjectManager {

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager&) = delete;
	void operator= (const GameObjectManager&) = delete;

public:
	static GameObjectManager& Instance();
	void Update();
	void Shutdown();
	void Add(const std::shared_ptr<GameObject> gameObject);
	void Clear();

private:
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;
};

