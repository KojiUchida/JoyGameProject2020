#include "TestScene2.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "Base/DirectXManager.h"
#include "GameObject/Event/StartCall.h"
#include "GameObject/Particle/ParticleManager.h"
#include "Game/Ranking.h"
#include <iostream>

TestScene2::TestScene2() : 
	m_objManager(GameObjectManager::Instance())
{
}

TestScene2::~TestScene2() {
}

void TestScene2::Init()
{
	auto& cam = Camera::Instance();
	cam.SetPosition(Vector3(0, 0, -10));

	Ranking::Instance().ResetRanking();
}

void TestScene2::Update()
{
	auto& cam = Camera::Instance();
	auto rotx = -Input::RightStickValue().y;
	auto roty = Input::RightStickValue().x;
	auto rot = Vector3(rotx, roty, 0) * 180.0f * GameTime::DeltaTime();

	float movex = Input::LeftStickValue().x;
	float movez = Input::LeftStickValue().y;
	float movey = Input::IsButton(PadButton::R1) ? 0.1f : Input::IsButton(PadButton::L1) ? -0.1f : 0.0f;

	auto forward = Vector3(movex, movey, movez) * Matrix4::RotationFromQuaternion(cam.rotation);
	auto move = forward * 10.0f * GameTime::DeltaTime();

	cam.SetRotation(cam.GetRotation() + rot);
	cam.SetPosition(cam.GetPosition() + forward);

	if (Input::IsKeyDown(DIK_Z))
	{
		//eventManager->SetEvent(new StartCall());
		//ParticleManager::Instance().AddParticleEmitter(new WaterSprash(Vector3(0),300,330));

		double time = rand() % 1000;
		Ranking::Instance().SetRanking(time);
		//�f�o�b�O�p��
		Ranking::Instance().GetMyRanking(time);
		Ranking::Instance().GetAllRanking();
	}

	EventManager::Instance().update();
	//ParticleManager::Instance().update();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

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
}

void TestScene2::Shutdown()
{
	m_objManager.Shutdown();
	m_objManager.Clear();
}

std::string TestScene2::NextScene()
{
	return "Test";
}

bool TestScene2::IsEnd()
{
	return Input::IsKeyDown(DIK_SPACE);
}
