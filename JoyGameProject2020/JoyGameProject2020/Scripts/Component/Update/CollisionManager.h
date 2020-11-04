#pragma once
#include <memory>
#include <vector>

class Collider3D;
class CollisionManager {

private:
	CollisionManager();
	~CollisionManager();
	CollisionManager(const CollisionManager&) = delete;
	void operator=(const CollisionManager&) = delete;

public:
	static CollisionManager& Instance();

	void Update();
	void Add(std::shared_ptr<Collider3D> collider);

private:
	std::vector<std::shared_ptr<Collider3D>> m_colliders;
};

