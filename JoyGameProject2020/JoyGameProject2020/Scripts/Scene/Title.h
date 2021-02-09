#pragma once
#include "IScene.h"
#include "GameObject/Event/EventManager.h"

class GameObject;
class GameObjectManager;
class IXAudio2SourceVoice;
class Title :public IScene
{
public:
	Title();
	~Title();

	// IScene ����Čp������܂���
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