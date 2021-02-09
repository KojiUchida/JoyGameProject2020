#include "Stage1.h"
#include "Device/Input.h"
#include "Device/Camera.h"
#include "Device/GameTime.h"
#include "Device/SoundManager.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/Event/EventManager.h"
#include "GameObject/Event/HeightGage.h"
#include "GameObject/Event/TimerUI.h"
#include "GameObject/Event/StartCall.h"
#include "GameObject/Event/ClearCall.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Light.h"
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "Base/DirectXManager.h"
#include "Math/MathUtil.h"
#include "Game/Player.h"
#include "Game/Goal.h"
#include "Game/GameManager.h"
#include "Physics/BoxCollider3D.h"

Stage1::Stage1():
	m_camera(Camera::Instance()),
m_light(Light::Instance()),
m_gameManager(GameManager::Instance()),
m_objManager(GameObjectManager::Instance())
{
}

Stage1::~Stage1()
{
}

void Stage1::Init()
{
	m_camera.SetPosition(Vector3(0, 0, -10));
	m_light.SetRotate(Vector3(-90, 0, 0));

	m_gameManager.ChangeState(GameState::READY);
	player = std::make_shared<Player>();
	m_objManager.Add(player);
	
	auto ground = std::make_shared<GameObject>();
	ground->AddComponent(std::make_shared<Model>("cube"));
	ground->SetPosition(Vector3(0, -30, 0));
	ground->SetScale(Vector3(100, 25, 1));
	m_objManager.Add(ground);

	auto wall1 = std::make_shared<GameObject>();
	wall1->AddComponent(std::make_shared<Model>("cube"));
	wall1->SetPosition(Vector3(-120, 25, 0));
	wall1->SetScale(Vector3(50, 1000, 1));
	m_objManager.Add(wall1);

	auto wall2 = std::make_shared<GameObject>();
	wall2->AddComponent(std::make_shared<Model>("cube"));
	wall2->SetPosition(Vector3(120, 25, 0));
	wall2->SetScale(Vector3(50, 1000, 1));
	m_objManager.Add(wall2);

	Random rnd{};
	float height = 0.0f;
	for (int i = 0; i < 25; ++i) {
		const float wdif = 20;
		const float ydif = 10;
		const float heightInterval = 30;
		auto w = rnd.GetRand(-1.0f, 1.0f);
		auto y = rnd.GetRand(0.0f, 1.0f);

		height += heightInterval + ydif * y;

		auto obj1 = std::make_shared<GameObject>();
		obj1->AddComponent(std::make_shared<Model>("cube"));
		auto c1 = std::make_shared<BoxCollider3D>();
		c1->SetLayer(Layer::Obstacle);
		obj1->AddComponent(c1);
		obj1->SetTag("Obstacle");
		obj1->SetPosition(Vector3(-60 + wdif * w, height, 0));
		obj1->SetScale(Vector3(50, 1, 1));
		m_objManager.Add(obj1);

		auto obj2 = std::make_shared<GameObject>();
		obj2->AddComponent(std::make_shared<Model>("cube"));
		auto c2 = std::make_shared<BoxCollider3D>();
		c2->SetLayer(Layer::Obstacle);
		obj2->AddComponent(c2);
		obj2->SetTag("Obstacle");
		obj2->SetPosition(Vector3(60 + wdif * w, height, 0));
		obj2->SetScale(Vector3(50, 1, 1));

		m_objManager.Add(obj2);
	}

	auto goal = std::make_shared<Goal>();
	goal->SetPosition(Vector3(0, 900, 0));
	goal->SetScale(Vector3(1000, 1, 1));
	m_objManager.Add(goal);

	heightGage = new HeightGage(player->MaxGauge());
	heightGage->initialize();
	EventManager::Instance().SetEvent(heightGage);
	EventManager::Instance().SetEvent(new StartCall());

	isEnd = false;

	bgm = SoundManager::Instance().Play("game", true);
}

void Stage1::Update()
{
	m_gameManager.Update();
	if (m_gameManager.CompareState(GameState::READY) &&
		m_gameManager.TimeElapsedOnCurrentState() > 3) {
		m_gameManager.ChangeState(GameState::PLAY);
		EventManager::Instance().SetEvent(new TimerUI());
	}
	CamMove();
	heightGage->SetGage(player->GaugeRatio());
	EventManager::Instance().update();
	//GUIUpdate();
	if (Input::IsKeyDown(DIK_SPACE))
	{
		m_gameManager.ChangeState(GameState::GAMEOVER);
	}
}

void Stage1::Shutdown()
{
	bgm->Stop();
}

std::string Stage1::NextScene()
{
	if (m_gameManager.CompareState(GameState::GAMEOVER)) return "GameOver";
	return "Clear";
}

bool Stage1::IsEnd()
{
	/*return m_gameManager.CompareState(GameState::GOAL) &&
		Input::IsButtonDown(PadButton::R1);*/
	return m_gameManager.CompareState(GameState::GAMEOVER) || m_gameManager.CompareState(GameState::GOAL);
}

void Stage1::CamMove()
{
	if (!CanCamMove()) return;
	auto campos = player->GetPosition() + Vector3(0, 20, -70);
	campos = Vector3::Lerp(Vector3(m_camera.GetPosition()), campos, 0.12f);
	m_camera.SetPosition(campos);
}

bool Stage1::CanCamMove()
{
	if (m_gameManager.CompareState(GameState::GOAL)) return false;
	if (m_gameManager.CompareState(GameState::GAMEOVER))return false;
	return true;
}


void Stage1::GUIUpdate()
{
	ImGui::StyleColorsDark();
	ImGui::GetStyle().WindowRounding = 0;
	ImGui::GetStyle().FrameRounding = 0;

	ImGui::Begin("Camera Menu");
	ImGui::SetWindowSize(ImVec2(512, 128), ImGuiCond_::ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(32, 64), ImGuiCond_::ImGuiCond_Always);

	float campos[3] = { m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z };
	ImGui::DragFloat3("Camera Position", campos);
	m_camera.SetPosition(Vector3(campos[0], campos[1], campos[2]));

	float camrot[3] = { m_camera.GetRotation().x, m_camera.GetRotation().y, m_camera.GetRotation().z };
	ImGui::DragFloat3("Camera Rotation", camrot, 1);
	m_camera.SetRotation(Vector3(camrot[0], camrot[1], camrot[2]));

	ImGui::End();

	ImGui::Begin("Player State");
	ImGui::SetWindowSize(ImVec2(512, 128), ImGuiCond_::ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(32, 256), ImGuiCond_::ImGuiCond_Always);

	ImGui::Text("Gauge : %f", player->Gauge());

	ImGui::End();

	ImGui::Begin("Debug");
	ImGui::SetWindowSize(ImVec2(512, 128), ImGuiCond_::ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(32, 448), ImGuiCond_::ImGuiCond_Always);

	ImGui::Text("FPS : %d", (int)GameTime::FPS());

	ImGui::Text("TimeElapsed : %f", m_gameManager.TimeElapsedOnCurrentState());

	ImGui::End();
}