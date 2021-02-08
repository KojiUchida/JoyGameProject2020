#include "BabbleFade.h"

#include "ClearCall.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Math/Vector3.h"
#include "Graphics/Sprite.h"
#include "Math/Easing.h"
#include "Math/MathUtil.h"
#include "Def/Screen.h"
#include "Device/GameTime.h"
#include "Math/Easing.h"

BabbleFade::BabbleFade()
{
}

BabbleFade::~BabbleFade()
{
}

void BabbleFade::initialize()
{
	m_objManager = std::make_shared<GameObjectManager>();
}

void BabbleFade::update()
{
	m_objManager->Update();
}

bool BabbleFade::IsEnd()
{
	return false;
}
