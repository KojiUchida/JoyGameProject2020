#pragma once
#include "iEvent.h"
#include "Math/Vector3.h"

class GameObject;
class GameObjectManager;

class HeightGage :public iEvent
{
public:
	HeightGage(const float&maxgage);
	~HeightGage();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

	void SetGage(const float& currentGage);

private:
	GameObjectManager& m_objManager;
	std::shared_ptr<GameObject> heightgagepointer;
	std::shared_ptr<GameObject> gagesprite;

	enum Direction
	{
		NONE, UP, DOWN
	};

	Direction direction;

	float speed;
	float rate;
	float maxnum;

	Vector3 pointermaxPos;
	Vector3 pointerminPos;

};
