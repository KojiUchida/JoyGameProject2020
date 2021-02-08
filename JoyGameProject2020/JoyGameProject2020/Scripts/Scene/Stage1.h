#pragma once

#pragma once
#include "IScene.h"
#include "GameObject/Event/ClearCall.h"
#include "GameObject/Event/StartCall.h"
#include "GameObject/Event/TimerUI.h"

class GameObject;
class GameObjectManager;
class Stage1 :public IScene
{
public:
	Stage1() {};
	~Stage1() {};

	// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;
	ClearCall*clearcall;
	StartCall*startcall;

	bool isEnd;
};