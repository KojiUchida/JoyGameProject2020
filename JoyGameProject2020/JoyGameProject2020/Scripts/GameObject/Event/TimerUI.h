#pragma once
#include "iEvent.h"
#include "Utility/Timer.h"

class GameObject;
class GameObjectManager;
class Sprite;
class TimerUI :public iEvent
{
public:
	TimerUI() {};
	~TimerUI();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager> m_objManager;
	std::shared_ptr<GameObject> number;
	std::shared_ptr<Sprite> n;
	Timer*timer;
};
