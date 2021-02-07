#pragma once
#include <memory>
#include "IScene.h"
#include "Math/Vector3.h"
#include "Utility/Random.h"

class GameObject;
class GameObjectManager;
class Camera;
class Player;
class Light;
class Sprite;
class GameManager;
class GameObjectManager;
class Timer;
class TestScene : public IScene {

public:
	TestScene();
	~TestScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	virtual std::string NextScene() override;
	virtual bool IsEnd() override;

	void CamMove();
	bool CanCamMove();

private:
	void GUIUpdate();

private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Sprite> sprite;

	Camera& m_camera;
	Light& m_light;
	GameManager& m_gameManager;
	GameObjectManager& m_objManager;
	bool m_showDebugMenu;
};

