#pragma once
#include "iEvent.h"

class GameObject;
class GameObjectManager;

class HeightGage :public iEvent
{
public:
	HeightGage() {};
	~HeightGage();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager> m_objManager;
	std::shared_ptr<GameObject> heightgagepointer;
	
};