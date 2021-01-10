#include "BoxCollider3D.h"
#include "CircleCollider3D.h"
#include "Math/Matrix4.h"

BoxCollider3D::BoxCollider3D() :
	Collider3D(ColliderType3D::Box) {
}

BoxCollider3D::~BoxCollider3D() {
}

bool BoxCollider3D::CheckCollision(Collider3D* other) {
	if (other->GetType() == ColliderType3D::Box) return CollisionBox((BoxCollider3D*)other);
	if (other->GetType() == ColliderType3D::Circle) return CollisionCircle((CircleCollider3D*)other);
	return false;
}

bool BoxCollider3D::CollisionBox(BoxCollider3D* other) {
	auto NAe1 = Right(), Ae1 = NAe1 * GetSize().x;
	auto NAe2 = Top(), Ae2 = NAe2 * GetSize().y;
	auto NAe3 = Front(), Ae3 = NAe3 * GetSize().z;
	auto NBe1 = other->Right(), Be1 = NBe1 * other->GetSize().x;
	auto NBe2 = other->Top(), Be2 = NBe2 * other->GetSize().y;
	auto NBe3 = other->Front(), Be3 = NBe3 * other->GetSize().z;
	auto interval = other->GetPosition() - GetPosition();

	//•ª—£Ž² : Ae1
	auto rA = Ae1.Length();
	auto rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	auto L = fabsf(Vector3::Dot(interval, NAe1));
	if (L > rA + rB) return false;

	//•ª—£Ž² : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(Vector3::Dot(interval, NAe2));
	if (L > rA + rB) return false;

	//•ª—£Ž² : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(Vector3::Dot(interval, NAe3));
	if (L > rA + rB) return false;

	//•ª—£Ž² : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabsf(Vector3::Dot(interval, NBe1));
	if (L > rA + rB) return false;

	//•ª—£Ž² : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabsf(Vector3::Dot(interval, NBe2));
	if (L > rA + rB) return false;

	//•ª—£Ž² : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabsf(Vector3::Dot(interval, NBe3));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C11
	auto Cross = Vector3::Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	//•ª—£Ž² : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::Dot(interval, Cross));
	if (L > rA + rB) return false;

	return true;
}

bool BoxCollider3D::CollisionCircle(CircleCollider3D* other) {
	auto len = LengthToPoint(other->GetPosition());
	return other->GetSize().x > len;
}

float BoxCollider3D::LenSegOnSeparateAxis(const Vector3& sep, const Vector3& e1, const Vector3& e2, const Vector3& e3) {
	float r1 = fabsf(Vector3::Dot(sep, e1));
	float r2 = fabsf(Vector3::Dot(sep, e2));
	float r3 = fabsf(Vector3::Dot(sep, e3));
	return r1 + r2 + r3;
}

float BoxCollider3D::LengthToPoint(const Vector3& p) {
	Vector3 v = Vector3::Zero();

	for (auto i = 0; i < 3; ++i) {
		auto L = i == 0 ? GetSize().x : i == 1 ? GetSize().y : i == 2 ? GetSize().z : 0;
		if (L == 0) continue;
		auto d = i == 0 ? Right() : i == 1 ? Top() : i == 2 ? Front() : 0;
		auto s = fabsf(Vector3::Dot(p - GetPosition(), d) / L);
		if (s > 1) {
			v += (1 - s) * L * d;
		}
	}

	return v.Length();
}
