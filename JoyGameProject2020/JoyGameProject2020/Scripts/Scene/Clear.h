#pragma once
#include "IScene.h"

class GameObject;
class GameObjectManager;
class IXAudio2SourceVoice;
class Clear :public IScene
{
public:
	Clear();
	~Clear();

	// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;

	virtual void Update() override;

	virtual void Shutdown() override;

	virtual std::string NextScene() override;

	virtual bool IsEnd() override;

	virtual void GUIUpdate() override;

private:
	GameObjectManager& m_objManager;
	IXAudio2SourceVoice* bgm;
};