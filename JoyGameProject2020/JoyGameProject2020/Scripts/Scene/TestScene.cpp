#include "TestScene.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Component/Draw/ObjRenderer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "Base/DirectXManager.h"
#include "Math/MathUtility.h"
#include <iostream>

TestScene::TestScene() :
	m_camera(Camera::Instance()) {
}

TestScene::~TestScene() {
}

void TestScene::Init() {
	m_camera.SetPosition(Vector3(0, 0, -10));

	m_objManager = std::make_shared<GameObjectManager>();

	m_obj1 = std::make_shared<GameObject>();
	m_obj1->AddComponent(std::make_shared<ObjRenderer>("skydome"));
	m_obj1->SetRotation(Vector3(180, 180, 0));
	m_obj1->SetScale(Vector3(5.0f));
	m_objManager->Add(m_obj1);

	m_obj2 = std::make_shared<GameObject>();
	m_obj2->AddComponent(std::make_shared<ObjRenderer>("dosei"));
	m_obj2->SetRotation(Vector3(0, 180, 0));
	m_obj2->SetScale(Vector3(0.03f));
	m_objManager->Add(m_obj2);

	auto obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<ObjRenderer>("dosei"));
	obj->SetRotation(Vector3(0, 180, 0));
	obj->SetScale(Vector3(0.03f));
	obj->SetPosition(Vector3(1, 0, 0));
	m_objManager->Add(obj);

	m_obj3 = std::make_shared<GameObject>();
	m_obj3->AddComponent(std::make_shared<ObjRenderer>("ground"));
	m_obj3->SetPosition(Vector3(0, -3, 0));
	m_obj3->SetScale(Vector3(10, 1, 10));
	m_objManager->Add(m_obj3);
}

void TestScene::Update() {

	if (Input::IsButtonDown(PadButton::A)) m_isStart = !m_isStart;

	/* ƒvƒŒƒCƒ„[‚Ì“®‚« */
	auto up = Vector3(0, 1, 0) * Matrix4::RotateZ(m_obj2->GetRotation().z);
	auto accX = MathUtility::IsZero(up.x) ? -m_velocity.x * 0.01f : up.x > 0 ? -0.3f : 0.3f;
	auto accY = up.y * m_isStart ? 0.5f : -0.5f;
	m_velocity += Vector3(accX, accY, 0);
	m_velocity.y = MathUtility::Clamp(m_velocity.y, 0, 20);
	auto vel = m_velocity;
	m_obj2->SetPosition(m_obj2->GetPosition() + m_velocity * GameTime::DeltaTime());

	auto rot = Vector3(0, 0, -Input::Gyro().z);
	m_velocity.y = MathUtility::Clamp(m_velocity.y, 0, 20);
	m_obj2->SetRotation(m_obj2->GetRotation() + rot);


	if (m_obj2->GetPosition().y <= 0) {
		m_obj2->SetPosition(m_obj2->GetPosition() * Vector3(1, 0, 1));
		m_velocity = 0;
	}




	m_camera.SetPosition(m_obj2->GetPosition() + Vector3(0, 0, -30));
	m_obj1->SetPosition(m_camera.GetPosition());
	m_objManager->Update();

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

	float campos[3] = { m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z };
	ImGui::DragFloat3("Camera Position", campos);
	m_camera.SetPosition(Vector3(campos[0], campos[1], campos[2]));

	float camrot[3] = { m_camera.GetRotation().x, m_camera.GetRotation().y, m_camera.GetRotation().z };
	ImGui::DragFloat3("Camera Rotation", camrot, 1);
	m_camera.SetRotation(Vector3(camrot[0], camrot[1], camrot[2]));

	ImGui::End();
}
