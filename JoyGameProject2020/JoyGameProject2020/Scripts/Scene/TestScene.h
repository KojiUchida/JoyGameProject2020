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
class TestScene : public IScene {

public:
	TestScene();
	~TestScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	virtual std::string NextScene() override;
	virtual bool IsEnd() override;

private:
	void GUIUpdate();

private:
	std::shared_ptr<GameObjectManager> m_objManager;
	std::shared_ptr<Player> player;

	Camera& m_camera;
	Light& m_light;
	bool m_isFreeCam;
	bool m_showDebugMenu;
};

