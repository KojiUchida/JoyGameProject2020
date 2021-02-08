#include "HeightGage.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"
#include "Device/Input.h"
#include "Math/MathUtil.h"

HeightGage::~HeightGage()
{
}

void HeightGage::initialize()
{
	m_objManager = std::make_shared<GameObjectManager>();

	num = 1;
	maxnum = 500;
	direction = NONE;
	speed = 1.0f;

	gagesprite = std::make_shared<GameObject>();
	gagesprite->AddComponent(std::make_shared<Sprite>("pink"));
	gagesprite->SetScale(Vector3(58, 0, 1));
	gagesprite->SetPosition(Vector3(Screen::WIDTH - 95, 215, 0));
	m_objManager->Add(gagesprite);

	auto heightgage = std::make_shared<GameObject>();
	heightgage->AddComponent(std::make_shared<Sprite>("bombe"));
	heightgage->SetScale(Vector3(128, 512, 0));
	heightgage->SetPosition(Vector3(Screen::WIDTH - 128, 100, 0));
	m_objManager->Add(heightgage);



}

void HeightGage::update()
{
	if (Input::IsKey(DIK_A))
	{
		direction = UP;
	}
	else
	{
		direction = DOWN;
	}

	switch (direction)
	{
	case HeightGage::NONE:
		break;

	case HeightGage::UP:
		num = (num < maxnum) ? num + speed : maxnum;
		break;

	case HeightGage::DOWN:
		num = (num > 0) ? num - speed : 0;
		break;
	}

	float rate = num / maxnum;
	gagesprite->SetPosition(Vector3::Lerp(Vector3(Screen::WIDTH - 95, 570, 0), Vector3(Screen::WIDTH - 95, 215, 0), rate));
	gagesprite->SetScale(Vector3::Lerp(Vector3(58, 0, 1),Vector3(58,570-215,0),rate));
	m_objManager->Update();


}

bool HeightGage::IsEnd()
{
	return false;
}
