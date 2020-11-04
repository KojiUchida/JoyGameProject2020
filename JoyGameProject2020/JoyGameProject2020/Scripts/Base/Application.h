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
	/* ������ */
	bool Init();
	/* �N�� */
	void Run();
	/* �I�� */
	void Shutdown();
	/* �j�� */
	void Quit();

private:
	/* �E�B���h�E�̐��� */
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

