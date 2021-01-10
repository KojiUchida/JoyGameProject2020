#pragma once
#include "Collider3D.h"

class CircleCollider3D;
class BoxCollider3D : public Collider3D {
public:
	BoxCollider3D();
	~BoxCollider3D();
	float LengthToPoint(const Vector3& p);

private:
	virtual bool CheckCollision(Collider3D* other) override;
	bool CollisionBox(BoxCollider3D* other);
	bool CollisionCircle(CircleCollider3D* other);
	float LenSegOnSeparateAxis(const Vector3& sep, const Vector3& e1, const Vector3& e2, const Vector3& e3 = 0);
};

