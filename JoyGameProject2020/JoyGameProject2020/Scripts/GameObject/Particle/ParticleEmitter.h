#pragma once
#include "Math/Vector3.h"
#include "GameObject/Particle/Particle.h"
#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	virtual void initialize()=0;//初期化
	virtual void update() {};//particle以外の更新()
	virtual void particleupdate();//particle用の更新

	virtual void create()=0;
	virtual bool IsEnd()=0;//終了条件

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