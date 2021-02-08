#include "StartCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Math/Easing.h"
#include "Math/MathUtil.h"
#include "Def/Screen.h"
#include <iostream>

StartCall::StartCall()
{
	timer = new Timer(2, false);
}

StartCall::~StartCall()
{
	delete(timer);
}

void StartCall::initialize()
{
	direction = FIRST;
	m_objManager = std::make_shared<GameObjectManager>();
	posMoverate = 1;
	firstpos = Vector3(Screen::WIDTH, Screen::HEIGHT / 2 - 64, 0);
	endpos = Vector3(Screen::WIDTH / 2 - 320, Screen::HEIGHT / 2 - 64, 0);

	ready = std::make_shared<GameObject>();
	ready->AddComponent(std::make_shared<Sprite>("ready"));
	ready->SetScale(Vector3(640, 128, 1));
	ready->SetPosition(firstpos);
	m_objManager->Add(ready);
	/*ready_back = std::make_shared<GameObject>();
	ready_back->AddComponent(std::make_shared<Sprite>("ready_back"));
	ready_back->SetScale(Vector3(1280, 1, 1));
	ready_back->SetPosition(Vector3(0, Screen::HEIGHT / 2 - 64, 1));
	m_objManager->Add(ready_back);
*/
	timer->Start();
}

void StartCall::update()
{
	timer->Update();
	float nowtime = timer->GetRatioElapsed();
	posMoverate = Easing::EaseOutQuint(nowtime);
	Vector3 obj1pos(0);
	obj1pos.x = MathUtil::Lerp((double)firstpos.x, (double)endpos.x, posMoverate);
	obj1pos.x = MathUtil::Clamp(obj1pos.x, endpos.x, firstpos.x);
	obj1pos.y = firstpos.y;

	ready->SetPosition(obj1pos);

	if (timer->IsTime()&&direction==FIRST) {
		ready->Destroy();

		go = std::make_shared<GameObject>();
		go->AddComponent(std::make_shared<Sprite>("go"));
		go->SetScale(Vector3(520, 130, 1));
		go->SetPosition(Vector3(Screen::WIDTH / 2 - 256, Screen::HEIGHT / 2 - 64, 0));
		m_objManager->Add(go);

		timer->Reset();
		timer->SetLimit(1);
		direction = SECOND;
	}

	float maxheight = 128;
	float scaleY = MathUtil::Lerp(0.0, (double)maxheight, Easing::EaseOutQuint(nowtime) * 2);
	scaleY = MathUtil::Clamp(scaleY, 0.0f, maxheight);

//	ready_back->SetScale(Vector3(1280,scaleY,1));

	m_objManager->Update();
}

bool StartCall::IsEnd()
{
	return timer->IsTime()&&direction==SECOND;
}
