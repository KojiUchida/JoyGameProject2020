#include "StageSerect.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/MathUtil.h"
#include "Device/Input.h"
#include "Device/GameTime.h"
#include "Device/SoundManager.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/Event/EventManager.h"
#include "GameObject/Event/StartCall.h"
#include "GameObject/Event/TimerUI.h"
#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"

#define PI 3.14159265

StageSerect::StageSerect():
	cam(Camera::Instance()),
m_objManager(GameObjectManager::Instance())
{
}

StageSerect::~StageSerect()
{
}

void StageSerect::Init()
{
	centerpos = Vector3(0);
	count = 1;
	stagenum = 5;
	stage_r = 10;

	stages.resize(stagenum);

	float camera_angle = count * (360 / stagenum);
	float x = (stage_r + 20) * cos(PI / 180 * camera_angle);
	float z = (stage_r + 20) * sin(PI / 180 * camera_angle);

	cam.SetPosition(Vector3(x, 0, z));
	cam.SetTarget(centerpos);

	for (int i = 0; i < stagenum; ++i)
	{
		stages[i] = std::make_shared<GameObject>();
		stages[i]->AddComponent(std::make_shared<Model>("chest"));
		float stage_angle = i * (360 / stagenum);
		float x = stage_r * cos(PI / 180 * stage_angle);
		float z = stage_r * sin(PI / 180 * stage_angle);

		float roty = atan2(centerpos.z - z, centerpos.x - x);
		roty = 180 / PI * roty;

		stages[i]->SetPosition(Vector3(x, 0, z));
		stages[i]->SetScale(Vector3(1));
		stages[i]->SetRotation(Vector3(0, -roty, 0));
		m_objManager.Add(stages[i]);
	}

	arrowscale = Vector3(32,32,1);

	LeftArrow = std::make_shared<GameObject>();
	LeftArrow->AddComponent(std::make_shared<Sprite>("LeftArrow"));

	LeftArrow->SetPosition(Vector3(Screen::WIDTH / 2-200-32, 560, 0));
	LeftArrow->SetScale(arrowscale);
	m_objManager.Add(LeftArrow);

	RightArrow = std::make_shared<GameObject>();
	RightArrow->AddComponent(std::make_shared<Sprite>("RightArrow"));

	RightArrow->SetPosition(Vector3(Screen::WIDTH / 2+ 200, 560, 0));
	RightArrow->SetScale(arrowscale);
	m_objManager.Add(RightArrow);

	stageName = std::make_shared<GameObject>();
	stageName->AddComponent(std::make_shared<Sprite>("stagename"));

	stageName->SetPosition(Vector3(Screen::WIDTH/2-74, 560, 0));
	stageName->SetScale(Vector3(128, 32, 1));
	m_objManager.Add(stageName);

	stagenumber = std::make_shared<GameObject>();
	stagenumber_s = std::make_shared<Sprite>("0");
	stagenumber->AddComponent(stagenumber_s);

	stagenumber->SetPosition(Vector3(Screen::WIDTH / 2+32, 560, 0));
	stagenumber->SetScale(Vector3(16, 32, 1));
	m_objManager.Add(stagenumber);

	bgm = SoundManager::Instance().Play("select", true);
}

void StageSerect::Update()
{
	if (Input::IsKeyDown(DIK_LEFT))
	{
		count = (count <= 1) ? stagenum : count - 1;
		LeftArrow->SetScale(arrowscale + Vector3(40, 40, 0));
	}
	else if (Input::IsKeyDown(DIK_RIGHT))
	{
		count = (count >= stagenum) ? 1 : count + 1;
		RightArrow->SetScale(arrowscale + Vector3(40, 40, 0));
	}

	stagenumber_s->SetTexture(std::to_string(count));

	float camera_angle = count * (360 / stagenum);
	float x = (stage_r + 10) * cos(PI / 180 * camera_angle);
	float z = (stage_r + 10) * sin(PI / 180 * camera_angle);

	Vector3 endpos = Vector3(x, 0, z);
	Vector3 pos= Vector3::Lerp(cam.GetPosition(), endpos, (float)GameTime::DeltaTime()*3);
	cam.SetPosition(pos);
	
	float endrot = atan2(centerpos.z - endpos.z, centerpos.x - endpos.x);
	endrot = 180 / PI * -endrot;

	Vector3 Lscale = Vector3::Lerp(LeftArrow->GetScale(), arrowscale, (float)GameTime::DeltaTime() * 3);
	Vector3 Rscale = Vector3::Lerp(RightArrow->GetScale(), arrowscale, (float)GameTime::DeltaTime() * 3);
	LeftArrow->SetScale(Lscale);
	RightArrow->SetScale(Rscale);

	EventManager::Instance().update();
	//GUIUpdate();
}

void StageSerect::Shutdown()
{
	bgm->Stop();
	cam.UnlockTarget();
	m_objManager.Shutdown();
	m_objManager.Clear();
}

std::string StageSerect::NextScene()
{
	return "Stage"+std::to_string(count);
}

bool StageSerect::IsEnd()
{
	return Input::IsKeyDown(DIK_SPACE) ||
		Input::IsButtonDown(PadButton::R1);
}

void StageSerect::GUIUpdate()
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

	ImGui::Begin("StageSerect");
	ImGui::SetWindowSize(ImVec2(512, 96), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(32, 170), ImGuiCond_::ImGuiCond_FirstUseEver);

	ImGui::Text("CurrentSerect : Stage %d",count);
	ImGui::End();
}
