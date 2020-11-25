#pragma once
#include "iEvent.h"

class GameObject;
class GameObjectManager;

class ManualUI :public iEvent
{
public:
	ManualUI();
	~ManualUI();
	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;


	double posMoverate;
	double time;
};