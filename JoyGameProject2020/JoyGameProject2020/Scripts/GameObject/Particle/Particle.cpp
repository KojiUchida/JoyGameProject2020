#include "Particle.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Graphics/Model.h"

Particle::Particle() :
	m_objManager(GameObjectManager::Instance())
{
}

Particle::~Particle()
{
}

void Particle::Init()
{
	position = Vector3(0);

	obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<Model>("plane"));
	obj->SetRotation(Vector3(-90, 0, 0));
	obj->SetScale(Vector3(scale));

	obj->SetPosition(position);
	m_objManager.Add(obj);

}

void Particle::Update()
{
	position += velocity;
	obj->SetPosition(position);
	obj->SetScale(Vector3(scale));
	--lifetime;
}

bool Particle::IsEnd()
{
	return (scale <= 0 || lifetime <= 0);
}
