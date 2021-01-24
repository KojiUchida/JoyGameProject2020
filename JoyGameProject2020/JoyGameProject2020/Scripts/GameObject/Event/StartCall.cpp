#include "StartCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Model.h"
#include "Math/Easing.h"
#include "Math/MathUtil.h"
#include <iostream>

StartCall::StartCall()
{
}

StartCall::~StartCall()
{
	delete(timer);
}

void StartCall::initialize()
{
	m_objManager = std::make_shared<GameObjectManager>();
	posMoverate = 1;
	firstpos = Vector3(8, 0, 0);
	endpos = Vector3(-4, 0, 0);

	obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<Model>("plane"));
	obj->SetRotation(Vector3(-90, 0, 0));
	obj->SetScale(Vector3(0.6f * 5, 1, 0.6f));
	obj->SetPosition(firstpos);
	m_objManager->Add(obj);

	background = std::make_shared<GameObject>();
	background->AddComponent(std::make_shared<Model>("plane"));
	background->SetRotation(Vector3(-90, 0, 0));
	background->SetScale(Vector3(30, 1, 0));
	background->SetPosition(Vector3(0, 0, 1));
	m_objManager->Add(background);

	timer = new Timer(2, false);
	timer->Start();
}

void StartCall::update()
{
	timer->Update();
	float nowtime = timer->GetRatioElapsed();
	posMoverate = Easing::EaseOutQuint(nowtime);
	Vector3 obj1pos(0);
	obj1pos.x = Math::Lerp((double)firstpos.x, (double)endpos.x, posMoverate);
	obj1pos.x = Math::Clamp(obj1pos.x, endpos.x, firstpos.x);

	obj->SetPosition(obj1pos);

	float maxheight = 1.5f;
	float background_scaleZ = Math::Lerp(0.0, (double)maxheight, Easing::EaseOutQuint(nowtime) * 2);
	background_scaleZ = Math::Clamp(background_scaleZ, 0.0f, maxheight);

//	background->SetScale(Vector3(30, 1, background_scaleZ));

	m_objManager->Update();
}

bool StartCall::IsEnd()
{
	return timer->IsTime();
}
