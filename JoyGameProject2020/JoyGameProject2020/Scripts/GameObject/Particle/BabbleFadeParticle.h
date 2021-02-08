#pragma once
#include "ParticleEmitter.h"

class BabbleFadeParticle :public ParticleEmitter
{
public:
	BabbleFadeParticle();
	~BabbleFadeParticle();
	virtual void initialize() override;
	virtual void update() override;
	virtual void create() override;
	virtual bool IsEnd() override;

	float time;
	bool isEnd;

	float lifetime;
	float firstangle;
	float endangle;
};