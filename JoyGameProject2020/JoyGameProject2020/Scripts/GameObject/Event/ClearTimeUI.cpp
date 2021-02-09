#include "ClearTimeUI.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"
#include "Game/GameManager.h"
#include "Game/Ranking.h"
#include "Device/Input.h"

ClearTimeUI::ClearTimeUI() :
	m_objManager(GameObjectManager::Instance())
{
}

ClearTimeUI::~ClearTimeUI()
{
	for (int i = 0; i < 8;)
	{
		cleartime[i]->Destroy();
		ranking1[i]->Destroy();
		ranking2[i]->Destroy();
		ranking3[i]->Destroy();
		++i;
	}


	cleartimelavel->Destroy();
	rank1->Destroy();
	rank2->Destroy();
	rank3->Destroy();

}

void ClearTimeUI::initialize()
{
	int width = 300;
	int height = 400;
	int interval = 70;

	cleartimelavel = std::make_shared<GameObject>();
	cleartimelavel->AddComponent(std::make_shared<Sprite>("cleartime"));
	cleartimelavel->SetScale(Vector3(512, 64, 1));
	cleartimelavel->SetPosition(Vector3(Screen::WIDTH/2-256, 110, 0));
	m_objManager.Add(cleartimelavel);

	auto ranklabel = std::make_shared<GameObject>();
	ranklabel->AddComponent(std::make_shared<Sprite>("ranklabel"));
	ranklabel->SetScale(Vector3(512, 64, 1));
	ranklabel->SetPosition(Vector3(Screen::WIDTH / 2 - 256, height + interval * 0, 0));
	m_objManager.Add(ranklabel);

	rank1 = std::make_shared<GameObject>();
	rank1->AddComponent(std::make_shared<Sprite>("rank1"));
	rank1->SetScale(Vector3(128, 64, 1));
	rank1->SetPosition(Vector3(width, height + interval * 1, 0));
	m_objManager.Add(rank1);

	rank2 = std::make_shared<GameObject>();
	rank2->AddComponent(std::make_shared<Sprite>("rank2"));
	rank2->SetScale(Vector3(128, 64, 1));
	rank2->SetPosition(Vector3(width, height + interval * 2, 0));
	m_objManager.Add(rank2);

	rank3 = std::make_shared<GameObject>();
	rank3->AddComponent(std::make_shared<Sprite>("rank3"));
	rank3->SetScale(Vector3(128, 64, 1));
	rank3->SetPosition(Vector3(width, height + interval * 3, 0));
	m_objManager.Add(rank3);


	for (int i = 0; i < 7;)
	{
		cleartime[i] = std::make_shared<GameObject>();
		cleartimenumber[i] = std::make_shared<Sprite>("0");

		double rank1time = GameManager::Instance().GetClearTime();
		int nowcount = 100 * rank1time;
		int keta = pow(10, 6 - i);//取り出したい桁
		nowcount = nowcount % (keta * 10);
		int count = nowcount / keta;//取り出した数字

		std::string a = std::to_string(count);
		cleartimenumber[i]->SetTexture(a);
		cleartime[i]->AddComponent(cleartimenumber[i]);
		cleartime[i]->SetScale(Vector3(32, 64, 0));

		int x = (i < 5) ? i * 32 : (i + 1) * 32;
		cleartime[i]->SetPosition(Vector3(x + Screen::WIDTH / 2 - 128, 184, 0));
		m_objManager.Add(cleartime[i]);
		++i;
	}

	dot0 = std::make_shared<Sprite>("dot");
	cleartime[7] = std::make_shared<GameObject>();
	cleartime[7]->AddComponent(dot0);
	cleartime[7]->SetScale(Vector3(32, 64, 0));
	cleartime[7]->SetPosition(Vector3(5 * 32 + Screen::WIDTH / 2 - 128,184, 0));
	m_objManager.Add(cleartime[7]);

	for (int i = 0; i < 7;)
	{
		ranking1[i] = std::make_shared<GameObject>();
		firstnumber[i] = std::make_shared<Sprite>("0");

		double rank1time = Ranking::Instance().GetRankingTime(1);
		//double rank1time = 0;
		int nowcount = 100 * rank1time;
		int keta = pow(10, 6 - i);//取り出したい桁
		nowcount = nowcount % (keta * 10);
		int count = nowcount / keta;//取り出した数字

		std::string a = std::to_string(count);
		firstnumber[i]->SetTexture(a);
		ranking1[i]->AddComponent(firstnumber[i]);
		ranking1[i]->SetScale(Vector3(32, 64, 0));

		int x = (i < 5) ? i * 32 : (i + 1) * 32;
		ranking1[i]->SetPosition(Vector3(x + width + 128, height + interval * 1, 0));
		m_objManager.Add(ranking1[i]);
		++i;
	}

	dot1 = std::make_shared<Sprite>("dot");
	ranking1[7] = std::make_shared<GameObject>();
	ranking1[7]->AddComponent(dot1);
	ranking1[7]->SetScale(Vector3(32, 64, 0));
	ranking1[7]->SetPosition(Vector3(5 * 32 + width + 128, height + interval * 1, 0));
	m_objManager.Add(ranking1[7]);

	for (int i = 0; i < 7;)
	{
		ranking2[i] = std::make_shared<GameObject>();
		secondnumber[i] = std::make_shared<Sprite>("0");

		double rank2time = Ranking::Instance().GetRankingTime(2);
		//double rank2time = 0;
		int nowcount = 100 * rank2time;
		int keta = pow(10, 6 - i);//取り出したい桁
		nowcount = nowcount % (keta * 10);
		int count = nowcount / keta;//取り出した数字

		std::string a = std::to_string(count);
		secondnumber[i]->SetTexture(a);
		ranking2[i]->AddComponent(secondnumber[i]);
		ranking2[i]->SetScale(Vector3(32, 64, 0));

		int x = (i < 5) ? i * 32 : (i + 1) * 32;
		ranking2[i]->SetPosition(Vector3(x + width + 128, height + interval * 2, 0));
		m_objManager.Add(ranking2[i]);
		++i;
	}

	dot2 = std::make_shared<Sprite>("dot");
	ranking2[7] = std::make_shared<GameObject>();
	ranking2[7]->AddComponent(dot2);
	ranking2[7]->SetScale(Vector3(32, 64, 0));
	ranking2[7]->SetPosition(Vector3(5 * 32 + width + 128, height + interval * 2, 0));
	m_objManager.Add(ranking2[7]);


	for (int i = 0; i < 7;)
	{
		ranking3[i] = std::make_shared<GameObject>();
		thirdnumber[i] = std::make_shared<Sprite>("0");

		double rank3time = Ranking::Instance().GetRankingTime(3);
		//double rank3time = 0;
		int nowcount = 100 * rank3time;
		int keta = pow(10, 6 - i);//取り出したい桁
		nowcount = nowcount % (keta * 10);
		int count = nowcount / keta;//取り出した数字

		std::string a = std::to_string(count);
		thirdnumber[i]->SetTexture(a);
		ranking3[i]->AddComponent(thirdnumber[i]);
		ranking3[i]->SetScale(Vector3(32, 64, 0));

		int x = (i < 5) ? i * 32 : (i + 1) * 32;
		ranking3[i]->SetPosition(Vector3(x + width + 128, height + interval * 3, 0));
		m_objManager.Add(ranking3[i]);
		++i;
	}

	dot3 = std::make_shared<Sprite>("dot");
	ranking3[7] = std::make_shared<GameObject>();
	ranking3[7]->AddComponent(dot3);
	ranking3[7]->SetScale(Vector3(32, 64, 0));
	ranking3[7]->SetPosition(Vector3(5 * 32 + width + 128, height + interval * 3, 0));
	m_objManager.Add(ranking3[7]);


}

void ClearTimeUI::update()
{
}

bool ClearTimeUI::IsEnd()
{
	return Input::IsKeyDown(DIK_SPACE);
}
