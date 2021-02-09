#pragma once

#include "iEvent.h"

class GameObject;
class GameObjectManager;
class Sprite;
class ClearTimeUI :public iEvent
{
public:
	ClearTimeUI();
	~ClearTimeUI();

	virtual void initialize() override;
	virtual void update() override;
	virtual bool IsEnd() override;

private:
	GameObjectManager& m_objManager;
	std::shared_ptr<Sprite> cleartimenumber[8];
	std::shared_ptr<GameObject> cleartime[8];
	std::shared_ptr<Sprite> firstnumber[8];
	std::shared_ptr<GameObject> ranking1[8];
	std::shared_ptr<Sprite> secondnumber[8];
	std::shared_ptr<GameObject> ranking2[8];
	std::shared_ptr<Sprite> thirdnumber[8];
	std::shared_ptr<GameObject> ranking3[8];
	std::shared_ptr<Sprite> dot0;
	std::shared_ptr<Sprite> dot1;
	std::shared_ptr<Sprite> dot2;
	std::shared_ptr<Sprite> dot3;

	std::shared_ptr<GameObject> rank1;
	std::shared_ptr<GameObject> rank2;
	std::shared_ptr<GameObject> rank3;
	std::shared_ptr<GameObject> rankout;
	std::shared_ptr<GameObject> cleartimelavel;
};