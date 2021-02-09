#pragma once
#include "IScene.h"
#include "GameObject/Event/EventManager.h"

class GameObject;
class GameObjectManager;
class GameOver :public IScene
{
public:
	GameOver();
	~GameOver();

	// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	GameObjectManager& m_objManager;
	std::shared_ptr<GameObject> gameover;
};