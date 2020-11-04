#pragma once
#include <vector>
#include <memory>

class GameObject;
class GameObjectManager {

public:
	GameObjectManager();
	~GameObjectManager();

	void Update();
	void Shutdown();
	void Add(const std::shared_ptr<GameObject> gameObject);
	void Clear();

private:
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;
};

