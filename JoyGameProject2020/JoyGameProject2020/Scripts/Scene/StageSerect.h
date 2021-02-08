#pragma once
#include "IScene.h"
#include <vector>
#include "Math/Vector3.h"
#include "Device/Camera.h"

class GameObject;
class GameObjectManager;
class StageSerect :public IScene
{
public:
	StageSerect();
	~StageSerect();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	virtual std::string NextScene() override;
	virtual bool IsEnd() override;
	virtual void GUIUpdate() override;

private:

	std::shared_ptr<GameObjectManager>m_objManager;
	std::vector < std::shared_ptr<GameObject>>stages;
	std::shared_ptr<GameObject> player;
	std::shared_ptr<GameObject> LeftArrow;
	std::shared_ptr<GameObject> RightArrow;
	std::shared_ptr<GameObject> stageName;
	

	Camera&cam;

	int count;
	int stagenum;
	int stage_r;
	Vector3 centerpos;
	Vector3 camendpos;
	Vector3 arrowscale;
};