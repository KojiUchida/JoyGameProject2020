#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

class SceneManager;
class DirectXManager;
class GameTime;
class Input;
class GraphicsManager;
class CollisionManager;
class Application {

private:
	Application();
	~Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:
	static Application& Instance();
	/* 初期化 */
	bool Init();
	/* 起動 */
	void Run();
	/* 終了 */
	void Shutdown();
	/* 破棄 */
	void Quit();

private:
	/* ウィンドウの生成 */
	void CreateGameWindow();
	void InitImGui();

private:
	WNDCLASSEX m_windowClass;
	HWND m_hwnd;
	std::unique_ptr<SceneManager> m_sceneManager;
	DirectXManager& m_dxManager;
	GameTime& m_gameTime;
	Input& m_input;
	GraphicsManager& m_graphicsManager;
	CollisionManager& m_collisionManager;
};

