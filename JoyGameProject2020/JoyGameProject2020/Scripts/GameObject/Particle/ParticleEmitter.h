#pragma once
#include "Math/Vector3.h"
#include "GameObject/Particle/Particle.h"
#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	virtual void initialize()=0;//������
	virtual void update() {};//particle�ȊO�̍X�V()
	virtual void particleupdate();//particle�p�̍X�V

	virtual void create()=0;
	virtual bool IsEnd()=0;//�I������

	virtual void AddParticle(Particle* particle);
	virtual void EraseParticle(int i);

	virtual void SetPosition(Vector3 pos);
	 
	struct State
	{
		Vector3 position;
		unsigned int particlenum;
		unsigned int maxparticle;
		float lifetime;
		float createRate;
	};
	State state;

	std::vector<Particle*>particles;
};