#include "TimerUI.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"

TimerUI::TimerUI() :
	m_objManager(GameObjectManager::Instance())
{
}

TimerUI::~TimerUI()
{
	delete(timer);
}

void TimerUI::initialize()
{
	timer = new Timer(60, false);
	timer->Start();

	for (int i = 0; i < 7;)
	{
		number[i] = std::make_shared<GameObject>();
		n[i] = std::make_shared<Sprite>("0");
		number[i]->AddComponent(n[i]);
		number[i]->SetScale(Vector3(32, 64, 0));

		int x = (i < 5) ? i * 32 : (i + 1) * 32;
		number[i]->SetPosition(Vector3(x, 0, 0));
		m_objManager.Add(number[i]);
		++i;
	}
	dot = std::make_shared<Sprite>("dot");
	number[7] = std::make_shared<GameObject>();
	number[7]->AddComponent(dot);
	number[7]->SetScale(Vector3(32, 64, 0));
	number[7]->SetPosition(Vector3(5 * 32, 0, 0));
	m_objManager.Add(number[7]);
}

void TimerUI::update()
{
	timer->Update();

	for (int i = 0; i < 7;)
	{
		int nowcount = 100 * timer->GetElapsed();//現在時間
		int keta = pow(10, 6 - i);//取り出したい桁
		nowcount = nowcount % (keta * 10);
		int count = nowcount / keta;//取り出した数字

		std::string a = std::to_string(count);
		n[i]->SetTexture(a);
		++i;
	}
}

bool TimerUI::IsEnd()
{
	return false;
}
