#pragma once
#include "GameObject/GameObject.h"

class Goal : public GameObject {

public:
	Goal() {};
	~Goal() {};

	virtual void OnCollisionEnter(std::shared_ptr<GameObject> other) override;

};

