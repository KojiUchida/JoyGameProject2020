#include "CircleCollider3D.h"
#include "BoxCollider3D.h"

CircleCollider3D::CircleCollider3D() :
	Collider3D(ColliderType3D::Circle) {
}

CircleCollider3D::~CircleCollider3D() {
}

bool CircleCollider3D::CheckCollision(Collider3D* other) {
	if (other->GetType() == ColliderType3D::Circle) return CollisionCircle((CircleCollider3D*)other);
	if (other->GetType() == ColliderType3D::Box) return CollisionBox((BoxCollider3D*)other);
	return false;
}

bool CircleCollider3D::CollisionCircle(CircleCollider3D* other) {
	auto range = (GetSize().x + other->GetSize().x) / 2;
	auto length = (other->GetPosition() - GetPosition()).Length();

	return range >= length;
}

bool CircleCollider3D::CollisionBox(BoxCollider3D* other) {
	auto len = other->LengthToPoint(GetPosition());
	return GetSize().x > len;
}
