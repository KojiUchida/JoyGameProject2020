#include "StartCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Component/Draw/ObjRenderer.h"
#include "Math/Easing.h"
#include "Math/MathUtility.h"

StartCall::StartCall()
{
}

StartCall::~StartCall()
{
	delete(timer);
}

void StartCall::initialize()
{
	isEnd = false;

	m_objManager = std::make_shared<GameObjectManager>();

	posMoverate = 1;
	firstpos = Vector3(0, 0, 0);
	endpos = Vector3(-3, 0, 0);

	for (unsigned int i = 0; i < 5;)
	{
		objs[i] = std::make_shared<GameObject>();
		objs[i]->AddComponent(std::make_shared<ObjRenderer>("plane"));
		objs[i]->SetRotation(Vector3(-90, 0, 0));
		objs[i]->SetScale(Vector3(1));
		float x = firstpos.x + i;
		objs[i]->SetPosition(firstpos + Vector3(x, 0, 0));
		m_objManager->Add(objs[i]);

		++i;
	}

	timer = new Timer(50, false);
	timer->Start();
	time = 0;
}

void StartCall::update()
{
	timer->Update();
	float nowtime = timer->GetElapsed();
	posMoverate = Easing::EaseOutCubic(nowtime);
	Vector3 obj1pos;
	obj1pos.x = MathUtility::Lerp(firstpos.x, endpos.x, posMoverate);

	for (int i = 0; i < _countof(objs); ++i)
	{
		objs[i]->SetPosition(objs[i]->GetPosition() - Vector3(obj1pos.x + i, 0, 0));
	}

	m_objManager->Update();
}

bool StartCall::IsEnd()
{
	return isEnd;
}
