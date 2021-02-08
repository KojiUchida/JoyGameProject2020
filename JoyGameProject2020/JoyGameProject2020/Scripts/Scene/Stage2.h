#pragma once

#pragma once
#include "IScene.h"
#include "GameObject/Event/EventManager.h"
#include "GameObject/Event/HeightGage.h"


class GameObject;
class GameObjectManager;
class Stage2 :public IScene
{
public:
	Stage2() {};
	~Stage2() {};

	// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	std::shared_ptr<GameObjectManager>m_objManager;
};