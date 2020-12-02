#pragma once

#include "iEvent.h"

class GameObject;
class GameObjectManager;
class StartCall :public iEvent
{
public:
	StartCall();
	~StartCall();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;
	std::shared_ptr<GameObject>obj;
	std::shared_ptr<GameObject>background;

	Timer*timer;

	Vector3 firstpos;
	Vector3 endpos;
	double posMoverate;

	
};