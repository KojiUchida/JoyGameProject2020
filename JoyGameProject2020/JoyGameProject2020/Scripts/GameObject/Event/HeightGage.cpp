#include "HeightGage.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Def/Screen.h"

HeightGage::~HeightGage()
{
}

void HeightGage::initialize()
{
	m_objManager = std::make_shared<GameObjectManager>();

	auto heightgage = std::make_shared<GameObject>();
	heightgage->AddComponent(std::make_shared<Sprite>("heightgage"));
	heightgage->SetScale(Vector3(50,600,0));
	heightgage->SetPosition(Vector3(Screen::WIDTH-96, 60, 0));
	m_objManager->Add(heightgage);

	auto heightgagepointer = std::make_shared<GameObject>();
	heightgagepointer->AddComponent(std::make_shared<Sprite>("heightgagepointer"));
	heightgagepointer->SetScale(Vector3(20, 20, 0));
	heightgagepointer->SetPosition(Vector3(Screen::WIDTH - 80, 640, 0));
	m_objManager->Add(heightgagepointer);
}

void HeightGage::update()
{
	m_objManager->Update();
}

bool HeightGage::IsEnd()
{
	return false;
}
