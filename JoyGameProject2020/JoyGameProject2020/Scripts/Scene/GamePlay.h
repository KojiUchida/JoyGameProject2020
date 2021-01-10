#pragma once

#pragma once
#include "IScene.h"
#include "GameObject/Event/EventManager.h"

class GameObject;
class GameObjectManager;
class GamePlay :public IScene
{
public:
	GamePlay() {};
	~GamePlay() {};

	// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;

};