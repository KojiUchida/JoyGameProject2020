#pragma once
#include "Math/Vector3.h"
#include "Utility/Color4.h"

#include <memory>

class GameObject;
class GameObjectManager;
class Particle
{
public:
	Particle();
	~Particle();

	void Init();
	void Update();

	bool IsEnd();

	std::shared_ptr<GameObjectManager>m_objManager;
	std::shared_ptr <GameObject> obj;

	
	Vector3 position;
	Vector3 velocity;
	float scale;
	float angle;
	Color4 firstColor;
	Color4 endColor;
	float lifetime;
};