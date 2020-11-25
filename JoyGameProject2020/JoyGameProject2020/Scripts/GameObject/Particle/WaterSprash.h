#pragma once
#include "ParticleEmitter.h"

class WaterSprash :public ParticleEmitter
{
public:
	WaterSprash(Vector3 pos ,float firstangle, float endangle);
	~WaterSprash();
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