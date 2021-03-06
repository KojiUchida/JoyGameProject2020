#include "GameTimer.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Model.h"
#include "Math/Easing.h"
#include "Math/MathUtil.h"
#include <iostream>

GameTimer::GameTimer() : 
	m_objManager(GameObjectManager::Instance())
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::initialize()
{
	position = Vector3(0, 0, 0);

	for (int i = 0; i < _countof(obj);)
	{
		obj[i] = std::make_shared<GameObject>();
		obj[i]->AddComponent(std::make_shared<Model>("plane"));
		obj[i]->SetRotation(Vector3(-90, 0, 0));
		obj[i]->SetScale(Vector3(0.6f * 5, 1, 0.6f));
		obj[i]->SetPosition(position*i);
		m_objManager.Add(obj[i]);
		++i;
	}

	timer = new Timer(2, false);
	timer->Start();
}


void GameTimer::update()
{
}

bool GameTimer::IsEnd()
{
	return false;
}
