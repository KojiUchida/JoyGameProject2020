#pragma once
#include <vector>
#include "GameObject/Particle/ParticleEmitter.h"

class ParticleManager
{
public:
	ParticleManager() {};
	ParticleManager(const ParticleManager&) {};
	ParticleManager&operator=(const ParticleManager&) {};
	static ParticleManager&Instance();
	~ParticleManager();

	void initialize();
	void update();

	void AddParticleEmitter(ParticleEmitter* particle);
	void ElaceParticleEmitter(int place);

private:
	std::vector<ParticleEmitter*> emitters;
};