#pragma once
#include "iEvent.h"
#include "Utility/Timer.h"

class GameObject;
class GameObjectManager;
class Sprite;
class TimerUI :public iEvent
{
public:
	TimerUI();
	~TimerUI();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	GameObjectManager& m_objManager;
	std::shared_ptr<GameObject> number[8];
	std::shared_ptr<Sprite> n[7];
	std::shared_ptr<Sprite> dot;
	Timer*timer;
};
