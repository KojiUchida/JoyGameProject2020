#include "WaterSprash.h"

WaterSprash::WaterSprash(Vector3 pos, float firstangle,float endangle)
{
	state.position = pos;
	this->firstangle = firstangle;
	this->endangle = endangle;
}

WaterSprash::~WaterSprash()
{
}

void WaterSprash::initialize()
{
	state.particlenum = 2;
	state.createRate = 10;

	time = 0;
	create();
	isEnd = false;
	lifetime = 240;
}

void WaterSprash::update()
{
	++time;
	if (time >= state.createRate)
	{
		create();
		time = 0;
	}
	--lifetime;
}

bool WaterSprash::IsEnd()
{
	return lifetime<=0;
}

void WaterSprash::create()
{
	float angle = 0;
	for (int i = 0; i < state.particlenum; ++i)
	{
		float rnd = rand() % 100;

		float anglerect = endangle - firstangle;
		angle = firstangle + anglerect * (rnd / 100);
		float x = cos(3.1415f / 180 * angle);
		float y = sin(3.1415f / 180 * angle);

		float speed = rand() % 10*0.1f;



		Particle* p = new Particle();

		p->position = state.position;
		p->velocity = Vector3(x*speed,y*speed,0);
		p->scale = 0.1f;
		p->lifetime = 60;
		p->Init();
		AddParticle(p);

	}
}
