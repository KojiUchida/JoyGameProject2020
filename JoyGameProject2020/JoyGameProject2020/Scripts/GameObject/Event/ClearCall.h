#pragma once

#include "iEvent.h"

class GameObject;
class GameObjectManager;
class ClearCall :public iEvent
{
public:
	ClearCall();
	~ClearCall();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;
	std::shared_ptr<GameObject>clear;
	std::shared_ptr<GameObject>back;

	Vector3 back_endpos;
	float easingrate;

	Timer*timer;
};