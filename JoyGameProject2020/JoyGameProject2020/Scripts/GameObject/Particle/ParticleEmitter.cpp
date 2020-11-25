#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
	for (int i = 0; i < particles.size();)
	{
		delete(particles[i]);
		++i;
	}
	particles.clear();
}

void ParticleEmitter::particleupdate()
{
	for (int i = 0; i < particles.size();)
	{
		
		if (particles[i]->IsEnd())
		{
			EraseParticle(i);
			continue;
		} 
		particles[i]->Update();
		++i;
	}
}

void ParticleEmitter::AddParticle(Particle * particle)
{
	particles.emplace_back(particle);
}

void ParticleEmitter::EraseParticle(int place)
{
	delete(particles[place]);
	particles[place] = particles.back();
	particles.pop_back();
}

void ParticleEmitter::SetPosition(Vector3 pos)
{
	state.position = pos;
}
