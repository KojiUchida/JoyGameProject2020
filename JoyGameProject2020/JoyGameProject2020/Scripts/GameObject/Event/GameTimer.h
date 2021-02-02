#pragma once

#include "iEvent.h"

class GameObject;
class GameObjectManager;

class GameTimer :public iEvent 
{
public:
	GameTimer();
	~GameTimer();

	virtual void initialize() override;

	virtual void update() override;

	virtual bool IsEnd() override;


private:
	GameObjectManager& m_objManager;
	std::shared_ptr<GameObject>obj[3];



	Timer*timer;

	Vector3 position;

};