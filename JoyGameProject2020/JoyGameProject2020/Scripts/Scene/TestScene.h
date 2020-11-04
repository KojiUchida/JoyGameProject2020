#pragma once
#include <memory>
#include "IScene.h"

class GameObject;
class GameObjectManager;
class TestScene : public IScene {

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	virtual std::string NextScene() override;
	virtual bool IsEnd() override;

private:
	std::shared_ptr<GameObjectManager> m_objManager;
	std::shared_ptr<GameObject> m_obj1;
	std::shared_ptr<GameObject> m_obj2;
	std::shared_ptr<GameObject> m_obj3;
};

