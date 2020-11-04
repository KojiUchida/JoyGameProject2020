#include "SceneManager.h"
#include "IScene.h"

SceneManager::SceneManager() :
	m_isChanged(false) {
}

SceneManager::~SceneManager() {
}

void SceneManager::Update() {
	if (m_isChanged) {
		m_currentScene->Init();
		m_isChanged = false;
	}

	m_currentScene->Update();

	if (m_currentScene->IsEnd()) {
		Shutdown();
		ChangeScene(m_currentScene->NextScene());
	}
}

void SceneManager::Shutdown() {
	m_currentScene->Shutdown();
}

void SceneManager::AddScene(const std::shared_ptr<IScene> scene, const std::string& name) {
	m_sceneMap.emplace(name, scene);
}

void SceneManager::ChangeScene(const std::string& name) {
	_ASSERT_EXPR(m_sceneMap.count(name), L"ƒV[ƒ“–¼‚ªŠÔˆá‚Á‚Ä‚¢‚Ü‚·");
	m_currentScene = m_sceneMap[name];
	m_isChanged = true;
}
