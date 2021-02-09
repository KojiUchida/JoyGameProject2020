#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

class SceneManager;
class DirectXManager;
class GameTime;
class Input;
class SoundManager;
class GraphicsManager;
class CollisionManager;
class GameObjectManager;
class SpriteRenderer;
class Renderer;
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
	SoundManager& m_soundManager;
	GraphicsManager& m_graphicsManager;
	CollisionManager& m_collisionManager;
	GameObjectManager& m_objManager;
	SpriteRenderer& m_spriteRenderer;
	Renderer& m_renderer;
};

