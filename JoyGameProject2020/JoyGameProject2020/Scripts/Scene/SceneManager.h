#pragma once
#include <memory>
#include <map>
#include <string>

class IScene;
class SceneManager {

public:
	SceneManager();
	~SceneManager();

	/* 更新 */
	void Update();
	/* 終了 */
	void Shutdown();

	/* シーンの登録 */
	void AddScene(const std::shared_ptr<IScene> scene, const std::string& name);
	/* シーンの変更 */
	void ChangeScene(const std::string& name);

private:
	bool m_isChanged;
	std::shared_ptr<IScene> m_currentScene;
	std::map<std::string, std::shared_ptr<IScene>> m_sceneMap;
};

