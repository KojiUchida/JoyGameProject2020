#pragma once
#include "Collider3D.h"

class BoxCollider3D;
class CircleCollider3D : public Collider3D {
public:
	CircleCollider3D();
	~CircleCollider3D();

private:
	virtual bool CheckCollision(Collider3D* other) override;
	bool CollisionCircle(CircleCollider3D* other);
	bool CollisionBox(BoxCollider3D* other);
};

