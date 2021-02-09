#include "GameOver.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Def/Screen.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "Graphics/Sprite.h"
#include "Math/Easing.h"
#include "Device/GameTime.h"

GameOver::GameOver() :
	m_objManager(GameObjectManager::Instance()) 
{
}

GameOver::~GameOver() 
{
}

void GameOver::Init()
{
	gameover = std::make_shared<GameObject>();
	gameover->AddComponent(std::make_shared<Sprite>("gameover"));
	gameover->SetScale(Vector3(512, 128, 1));
	gameover->SetPosition(Vector3(Screen::WIDTH / 2 - 256, -128, 1));
	m_objManager.Add(gameover);


}

void GameOver::Update()
{
	Vector3 endpos = Vector3(Screen::WIDTH / 2 - 256, 300, 1);

	float rate = Easing::EaseOutElastic(GameTime::DeltaTime());
	gameover->SetPosition(Vector3::Lerp(gameover->GetPosition(), endpos,rate));

	GUIUpdate();
}

void GameOver::Shutdown()
{
	m_objManager.Shutdown();
	m_objManager.Clear();
}

std::string GameOver::NextScene()
{
	return "StageSerect";
}

bool GameOver::IsEnd()
{
	return Input::IsKeyDown(DIK_SPACE);
}

void GameOver::GUIUpdate()
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

	ImGui::Begin("GameOver");
	ImGui::SetWindowSize(ImVec2(512, 96), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(32, 170), ImGuiCond_::ImGuiCond_FirstUseEver);


	ImGui::End();
}
