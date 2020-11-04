#pragma once
#include <memory>
#include <map>
#include <string>

class IScene;
class SceneManager {

public:
	SceneManager();
	~SceneManager();

	/* �X�V */
	void Update();
	/* �I�� */
	void Shutdown();

	/* �V�[���̓o�^ */
	void AddScene(const std::shared_ptr<IScene> scene, const std::string& name);
	/* �V�[���̕ύX */
	void ChangeScene(const std::string& name);

private:
	bool m_isChanged;
	std::shared_ptr<IScene> m_currentScene;
	std::map<std::string, std::shared_ptr<IScene>> m_sceneMap;
};

