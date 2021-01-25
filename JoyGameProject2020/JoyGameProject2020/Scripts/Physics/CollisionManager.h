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
	void Add(Collider3D* collider);

private:
	std::vector<Collider3D*> m_colliders;
};

