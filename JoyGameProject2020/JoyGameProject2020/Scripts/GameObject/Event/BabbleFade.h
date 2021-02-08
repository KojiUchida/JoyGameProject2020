#pragma once

#include "iEvent.h"
#include <vector>

class GameObject;
class GameObjectManager;
class BabbleFade :public iEvent
{
public:
	BabbleFade();
	~BabbleFade();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;

	std::vector< std::shared_ptr < GameObject>>babbles;

	Timer*timer;
};