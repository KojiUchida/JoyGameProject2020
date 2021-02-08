#include "ClearCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Math/Easing.h"
#include "Math/MathUtil.h"
#include "Def/Screen.h"
#include "Device/GameTime.h"
#include "Math/Easing.h"

ClearCall::ClearCall()
{
	timer = new Timer(3, false);
}

ClearCall::~ClearCall()
{
	delete(timer);
}

void ClearCall::initialize()
{
	m_objManager = std::make_shared<GameObjectManager>();

	clear = std::make_shared<GameObject>();
	clear->AddComponent(std::make_shared<Sprite>("clear"));
	clear->SetScale(Vector3(0, 0, 1));
	clear->SetPosition(Vector3(Screen::WIDTH/2, Screen::HEIGHT / 2,0));
	m_objManager->Add(clear);

	back = std::make_shared<GameObject>();
	back->AddComponent(std::make_shared<Sprite>("pink"));
	back->SetScale(Vector3(1280, 128, 1));
	back->SetPosition(Vector3(1280, Screen::HEIGHT / 2 - 64, 0));
	//m_objManager->Add(back);

	timer->Start();
	easingrate = 0;
}

void ClearCall::update()
{
	back->SetPosition(Vector3::Lerp(back->GetPosition(), Vector3(0, Screen::HEIGHT / 2 - 64, 0), GameTime::DeltaTime()*3));

	easingrate = Easing::EaseOutElastic(timer->GetRatioElapsed()*0.5f);
	clear->SetScale(Vector3::Lerp(clear->GetScale(), Vector3(512, 64, 1), easingrate));
	clear->SetPosition(Vector3::Lerp(clear->GetPosition(), Vector3(Screen::WIDTH / 2 - 256, Screen::HEIGHT / 2 - 32, 0),easingrate));
	
	m_objManager->Update();
	timer->Update();
}

bool ClearCall::IsEnd()
{
	return timer->IsTime();
}
