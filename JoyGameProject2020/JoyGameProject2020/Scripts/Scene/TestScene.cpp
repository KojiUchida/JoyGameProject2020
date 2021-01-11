#include "TestScene.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Light.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "Base/DirectXManager.h"
#include "Math/MathUtil.h"
#include "Game/Player.h"
#include <iostream>

TestScene::TestScene() :
	m_camera(Camera::Instance()),
	m_isFreeCam(false),
	m_light(Light::Instance()) {
}

TestScene::~TestScene() {
}

void TestScene::Init() {
	m_camera.SetPosition(Vector3(0, 0, -10));
	m_light.SetRotate(Vector3(-90, 0, 0));

	m_objManager = std::make_shared<GameObjectManager>();

	player = std::make_shared<Player>();
	m_objManager->Add(player);

	auto ground = std::make_shared<GameObject>();
	ground->AddComponent(std::make_shared<Model>("cube"));
	ground->SetPosition(Vector3(0, -30, 0));
	ground->SetScale(Vector3(100, 25, 1));
	m_objManager->Add(ground);

	auto wall1 = std::make_shared<GameObject>();
	wall1->AddComponent(std::make_shared<Model>("cube"));
	wall1->SetPosition(Vector3(-120, 25, 0));
	wall1->SetScale(Vector3(50, 1000, 1));
	m_objManager->Add(wall1);

	auto wall2 = std::make_shared<GameObject>();
	wall2->AddComponent(std::make_shared<Model>("cube"));
	wall2->SetPosition(Vector3(120, 25, 0));
	wall2->SetScale(Vector3(50, 1000, 1));
	m_objManager->Add(wall2);

	Random rnd{};
	float height = 0.0f;
	for (int i = 0; i < 30; ++i) {
		const float wdif = 20;
		const float ydif = 10;
		const float heightInterval = 30;
		auto w = rnd.GetRand(-1.0f, 1.0f);
		auto y = rnd.GetRand(0.0f, 1.0f);

		height += heightInterval + ydif * y;

		auto obj1 = std::make_shared<GameObject>();
		obj1->AddComponent(std::make_shared<Model>("cube"));
		obj1->SetPosition(Vector3(-60 + wdif * w, height, 0));
		obj1->SetScale(Vector3(50, 1, 1));
		m_objManager->Add(obj1);

		auto obj2 = std::make_shared<GameObject>();
		obj2->AddComponent(std::make_shared<Model>("cube"));
		obj2->SetPosition(Vector3(60 + wdif * w, height, 0));
		obj2->SetScale(Vector3(50, 1, 1));

		m_objManager->Add(obj2);
	}

	auto obj = std::make_shared<GameObject>();
	sprite = std::make_shared<Sprite>("nontan");
	sprite->SetTexture("ready");
	obj->AddComponent(sprite);
	obj->SetScale(200);
	m_objManager->Add(obj);
}

void TestScene::Update() {
	m_objManager->Update();
	if (!m_isFreeCam) {
		m_camera.SetPosition(player->GetPosition() + Vector3(0, 0, -100));
	}

	GUIUpdate();
}

void TestScene::Shutdown() {
	m_objManager->Shutdown();
}

std::string TestScene::NextScene() {
	return "Test";
}

bool TestScene::IsEnd() {
	return Input::IsKeyDown(DIK_SPACE);
}

void TestScene::GUIUpdate() {
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

	float campos[3] = { m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z };
	ImGui::DragFloat3("Camera Position", campos);
	m_camera.SetPosition(Vector3(campos[0], campos[1], campos[2]));

	float camrot[3] = { m_camera.GetRotation().x, m_camera.GetRotation().y, m_camera.GetRotation().z };
	ImGui::DragFloat3("Camera Rotation", camrot, 1);
	m_camera.SetRotation(Vector3(camrot[0], camrot[1], camrot[2]));

	ImGui::Checkbox("Free Camera", &m_isFreeCam);

	ImGui::End();

	ImGui::Begin("Player State");
	ImGui::SetWindowSize(ImVec2(512, 96), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(32, 256), ImGuiCond_::ImGuiCond_FirstUseEver);

	ImGui::Text("Gauge : %f", player->Gauge());

	ImGui::End();
}
