#include "Particle.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Component/Draw/ObjRenderer.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Init()
{
	position = Vector3(0);
	m_objManager = std::make_shared<GameObjectManager>();

	obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<ObjRenderer>("plane"));
	obj->SetRotation(Vector3(-90, 0, 0));
	obj->SetScale(Vector3(scale));

	obj->SetPosition(position);
	m_objManager->Add(obj);

}

void Particle::Update()
{
	position += velocity;
	obj->SetPosition(position);
	obj->SetScale(Vector3(scale));
	--lifetime;
	m_objManager->Update();
}

bool Particle::IsEnd()
{
	return (scale <= 0 || lifetime <= 0);
}
