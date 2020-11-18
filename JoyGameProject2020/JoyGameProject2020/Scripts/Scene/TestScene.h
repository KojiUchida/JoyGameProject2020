#pragma once
#include <memory>
#include "IScene.h"
#include "Math/Vector3.h"
#include "Utility/Random.h"

class GameObject;
class GameObjectManager;
class Camera;
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
	std::shared_ptr<GameObject> m_obj1;
	std::shared_ptr<GameObject> m_obj2;
	std::shared_ptr<GameObject> m_obj3;

	Camera& m_camera;
	bool m_isStart = false;
	Vector3 m_velocity;
	Random rnd;
};

