#include "ParticleManager.h"
#include <memory>

ParticleManager & ParticleManager::Instance()
{
	static std::unique_ptr<ParticleManager>instance(new ParticleManager());
	return *instance;
}

ParticleManager::~ParticleManager()
{
	for (int i = 0; i < emitters.size();)
	{
		delete(emitters[i]);
		++i;
	}
	emitters.clear();
}

void ParticleManager::initialize()
{
}

void ParticleManager::update()
{
	for (int i=0;i<emitters.size();)
	{
		if (emitters[i]->IsEnd())
		{
			ElaceParticleEmitter(i);
			continue;
		}
		emitters[i]->update();
		emitters[i]->particleupdate();
		++i;
	}
}

void ParticleManager::AddParticleEmitter(ParticleEmitter * particleEmitter)
{
	emitters.emplace_back(particleEmitter);
	particleEmitter->initialize();
}

void ParticleManager::ElaceParticleEmitter(int place)
{
	delete(emitters[place]);
	emitters[place] = emitters.back();
	emitters.pop_back();
}
