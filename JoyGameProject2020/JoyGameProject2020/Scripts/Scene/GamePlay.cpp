#include "GamePlay.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "Game/Player.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/Event/EventManager.h"
#include "GameObject/Event/StartCall.h"
#include "Graphics/Sprite.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"

GamePlay::GamePlay() :
	m_objManager(GameObjectManager::Instance()) 
{
}

GamePlay::~GamePlay() 
{
}

void GamePlay::Init()
{
	m_objManager.Add(std::make_shared<Player>());



	EventManager::Instance().SetEvent(new HeightGage());
}

void GamePlay::Update()
{
	if (Input::IsKeyDown(DIK_S))
	{
		EventManager::Instance().SetEvent(new StartCall());
	}

	EventManager::Instance().update();
	GUIUpdate();
}

void GamePlay::Shutdown()
{
	m_objManager.Shutdown();
	m_objManager.Clear();
}

std::string GamePlay::NextScene()
{
	return "Clear";
}

bool GamePlay::IsEnd()
{
	return Input::IsKeyDown(DIK_SPACE);
}

void GamePlay::GUIUpdate()
{
	auto& cam = Camera::Instance();

	ImGui::StyleColorsDark();
	ImGui::GetStyle().WindowRounding = 0;
	ImGui::GetStyle().FrameRounding = 0;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Border] = ImVec4(0, 1, 1, 1);
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 0.5f);
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TitleBgActive] = ImVec4(0, 1, 1, 0.5f);
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = ImVec4(0, 1, 1, 0.2f);
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Text] = ImVec4(0.5f, 1, 1, 1);

	ImGui::Begin("Camera Menu");
	ImGui::SetWindowSize(ImVec2(512, 96), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(32, 64), ImGuiCond_::ImGuiCond_FirstUseEver);

	float campos[3] = { cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z };
	ImGui::DragFloat3("Camera Position", campos);
	cam.SetPosition(Vector3(campos[0], campos[1], campos[2]));

	float camrot[3] = { cam.GetRotation().x, cam.GetRotation().y, cam.GetRotation().z };
	ImGui::DragFloat3("Camera Rotation", camrot, 1);
	cam.SetRotation(Vector3(camrot[0], camrot[1], camrot[2]));

	ImGui::End();

	ImGui::Begin("GamePlay");
	ImGui::SetWindowSize(ImVec2(512, 96), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(32, 170), ImGuiCond_::ImGuiCond_FirstUseEver);


	ImGui::End();
}
