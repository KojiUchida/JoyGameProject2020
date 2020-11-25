#include "StartCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Component/Draw/ObjRenderer.h"
#include "Math/Easing.h"
#include "Math/MathUtility.h"
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
	firstpos = Vector3(5, 0, 0);
	endpos = Vector3(-5, 0, 0);

	for (unsigned int i = 0; i < 1;)
	{
		objs[i] = std::make_shared<GameObject>();
		objs[i]->AddComponent(std::make_shared<ObjRenderer>("plane"));
		objs[i]->SetRotation(Vector3(-90, 0, 0));
		objs[i]->SetScale(Vector3(1));
		float x = firstpos.x + i*3;
		objs[i]->SetPosition(firstpos + Vector3(x, 0, 0));
		m_objManager->Add(objs[i]);

		++i;
	}

	timer = new Timer(2, false);
	timer->Start();
	time = 0;
}

void StartCall::update()
{
	timer->Update();
	++time;
	float nowtime = timer->GetRatioElapsed();
	posMoverate = Easing::EaseInCubic(nowtime);
	Vector3 obj1pos;
	obj1pos.x = MathUtility::Lerp(firstpos.x, endpos.x, posMoverate);
	obj1pos.x = MathUtility::Clamp(obj1pos.x, endpos.x, firstpos.x);

	for (int i = 0; i < _countof(objs); ++i)
	{
		objs[i]->SetPosition(Vector3(obj1pos.x + i, 0, 0));
	}

	m_objManager->Update();
}

bool StartCall::IsEnd()
{
	return time>480;
}
