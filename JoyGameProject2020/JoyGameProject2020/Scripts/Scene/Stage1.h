#pragma once

#include "IScene.h"
#include "Math/Vector3.h"
#include "Utility/Random.h"
#include "GameObject/Event/HeightGage.h"
#include <memory>

class GameObject;
class GameObjectManager;
class Camera;
class Player;
class Light;
class Sprite;
class GameManager;
class GameObjectManager;
class Timer;
class Stage1 :public IScene
{
public:
	Stage1();
	~Stage1();

	// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

	void CamMove();
	bool CanCamMove();

private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Sprite> sprite;

	Camera& m_camera;
	Light& m_light;
	GameManager& m_gameManager;
	GameObjectManager& m_objManager;
	bool m_showDebugMenu;

	HeightGage*heightGage;


	bool isEnd;
};