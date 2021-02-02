#include "Application.h"
#include "Def/Screen.h"
#include "DirectXManager.h"
#include "Device/Input.h"
#include "Device/GameTime.h"
#include "Scene/SceneManager.h"
#include "Scene/TestScene.h"
#include "Scene/Title.h"
#include "Scene/GamePlay.h"
#include "Scene/GameOver.h"
#include "Scene/Clear.h"
#include "GameObject/GameObjectManager.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Renderer.h"
#include "Physics/CollisionManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"

#define NEW ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Application::Application() :
	m_windowClass(),
	m_hwnd(),
	m_dxManager(DirectXManager::Instance()),
	m_gameTime(GameTime::Instance()),
	m_input(Input::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_collisionManager(CollisionManager::Instance()),
	m_objManager(GameObjectManager::Instance()),
	m_renderer(Renderer::Instance()) {
}

Application::~Application() {
}

Application& Application::Instance() {
	static Application instance;
	return instance;
}

bool Application::Init() {
#if _DEBUG
	/* �f�o�b�O���̓��������[�N�����o���o���� */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 

	CreateGameWindow();

	/* �������珉�������� */

	m_dxManager.Init(m_hwnd);

	InitImGui();

	m_input.Init(m_hwnd, m_windowClass.hInstance);
	m_gameTime.Init();

	m_graphicsManager.Init();

	m_graphicsManager.LoadTexture("Resources/Textures/ultnontan.jpg", "nontan");
	m_graphicsManager.LoadTexture("Resources/Textures/ready.png", "ready");
	m_graphicsManager.LoadTexture("Resources/Textures/ready_back.png", "ready_back");
	m_graphicsManager.LoadTexture("Resources/Textures/go.png", "go");
	m_graphicsManager.LoadTexture("Resources/Textures/title.png", "title");
	m_graphicsManager.LoadTexture("Resources/Textures/heightgage.png", "heightgage");
	m_graphicsManager.LoadTexture("Resources/Textures/heightgagepointer.png", "heightgagepointer");

	m_graphicsManager.LoadModel("Resources/Models/cube/cube.obj", "cube");
	m_graphicsManager.LoadModel("Resources/Models/dosei/dosei_quad.obj", "dosei");
	m_graphicsManager.LoadModel("Resources/Models/grass/grass.obj", "grass");
	m_graphicsManager.LoadModel("Resources/Models/ground/ground.obj", "ground");
	m_graphicsManager.LoadModel("Resources/Models/plane/plane.obj", "plane");
	m_graphicsManager.LoadModel("Resources/Models/skydome/skydome.obj", "skydome");
	m_graphicsManager.LoadModel("Resources/Models/sphere/sphere.obj", "sphere", true);

	m_graphicsManager.LoadShader("Resources/Shaders/SpriteVertexShader.hlsl", "VSmain", "vs_5_0", "SpriteVS");
	m_graphicsManager.LoadShader("Resources/Shaders/SpritePixelShader.hlsl", "PSmain", "ps_5_0", "SpritePS");

	m_graphicsManager.LoadShader("Resources/Shaders/BasicVertexShader.hlsl", "VSmain", "vs_5_0", "BasicVS");
	m_graphicsManager.LoadShader("Resources/Shaders/BasicPixelShader.hlsl", "PSmain", "ps_5_0", "BasicPS");

	m_renderer.Init();

	m_sceneManager = std::make_unique<SceneManager>();
	m_sceneManager->AddScene(std::make_shared<TestScene>(), "Test");
	//m_sceneManager->AddScene(std::make_shared<TestScene2>(), "Test2");
	m_sceneManager->AddScene(std::make_shared<Title>(), "Title");
	m_sceneManager->AddScene(std::make_shared<GamePlay>(), "GamePlay");
	m_sceneManager->AddScene(std::make_shared<GameOver>(), "GameOver");
	m_sceneManager->AddScene(std::make_shared<Clear>(), "Clear");
	m_sceneManager->ChangeScene("Test");

	/* �����܂ŏ��������� */

	return true;
}

void Application::Run() {
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}

		/* ��������X�V���� */

		m_input.Update();

		m_gameTime.Update();

		m_dxManager.BeginScene();

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		m_sceneManager->Update();
		m_objManager.Update();
		m_collisionManager.Update();
		m_renderer.Update();
		m_renderer.Draw();

		ImGui::Render();
		m_dxManager.GetCommandList()->SetDescriptorHeaps(1, m_dxManager.GetDescHeapForImGui().GetAddressOf());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_dxManager.GetCommandList().Get());

		m_dxManager.EndScene();

		/* �����܂ōX�V���� */
	}
}

void Application::Shutdown() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_sceneManager->Shutdown();
	m_input.Shutdown();
	UnregisterClass(m_windowClass.lpszClassName, m_windowClass.hInstance);
	ShowCursor(true);
}

void Application::Quit() {
	DestroyWindow(m_hwnd);
}

void Application::CreateGameWindow() {
	m_windowClass = {};
	m_windowClass.cbSize = sizeof(WNDCLASSEX);
	m_windowClass.lpfnWndProc = (WNDPROC)WindowProc;
	m_windowClass.lpszClassName = L"KojiLibrary";
	m_windowClass.hInstance = GetModuleHandle(nullptr);
	m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&m_windowClass);
	RECT wrc = { 0, 0, Screen::WIDTH, Screen::HEIGHT };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindow(
		m_windowClass.lpszClassName,
		L"KojiLibrary",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		m_windowClass.hInstance,
		nullptr
	);

	ShowWindow(m_hwnd, SW_SHOW);
}

void Application::InitImGui() {
	ImGui::CreateContext();
	auto result = ImGui_ImplWin32_Init(m_hwnd);
	_ASSERT_EXPR(result, L"ImGui�̏������Ɏ��s���܂���");
	result = ImGui_ImplDX12_Init(m_dxManager.GetDevice().Get(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		m_dxManager.GetDescHeapForImGui().Get(),
		m_dxManager.GetDescHeapForImGui()->GetCPUDescriptorHandleForHeapStart(),
		m_dxManager.GetDescHeapForImGui()->GetGPUDescriptorHandleForHeapStart()
	);
	_ASSERT_EXPR(result, L"ImGui�̏������Ɏ��s���܂���");
}
