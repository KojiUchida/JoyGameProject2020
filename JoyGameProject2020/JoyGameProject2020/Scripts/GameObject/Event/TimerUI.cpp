#include "TimerUI.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"

TimerUI::~TimerUI()
{
	delete(timer);
}

void TimerUI::initialize()
{
	timer = new Timer(60, false);
	timer->Start();

	m_objManager = std::make_shared<GameObjectManager>();

	n = std::make_shared<Sprite>();
	n->SetColor(Color4(1,1,1,1));
	number->AddComponent(n);
	number->SetScale(Vector3(50, 600, 0));
	number->SetPosition(Vector3(Screen::WIDTH - 96, 60, 0));
	m_objManager->Add(number);
}

void TimerUI::update()
{

}

bool TimerUI::IsEnd()
{
	return timer->IsTime();
}
