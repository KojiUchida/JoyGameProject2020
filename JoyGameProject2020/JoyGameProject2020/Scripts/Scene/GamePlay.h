#pragma once

#pragma once
#include "IScene.h"
#include "GameObject/Event/EventManager.h"
#include "GameObject/Event/HeightGage.h"


class GameObject;
class GameObjectManager;
class GamePlay : public IScene
{
public:
	GamePlay();
	~GamePlay();

	// IScene ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	GameObjectManager& m_objManager;
};