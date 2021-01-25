#pragma once
#include <memory>
#include "GameObject/Component.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "GameObject/GameObject.h"
#include "CollisionManager.h"
#include "PhysicsLayer.h"

enum class ColliderType3D {
	Circle,
	Box,
};

class CollisionManager;
class Collider3D : public Component {
	friend CollisionManager;
public:
	Collider3D(ColliderType3D type) :
		m_type(type),
		m_size(1),
		m_layer(Layer::None) {
	};
	~Collider3D() {};

	virtual void Update() override {
		auto& colMane = CollisionManager::Instance();
		colMane.Add(this);
		precollisionlist = collisionlist;
		collisionlist.clear();
	};

	virtual bool CheckCollision(Collider3D* other) = 0;

	bool IsPreCollide(Collider3D* col) { 
		return std::count(std::begin(precollisionlist), std::end(precollisionlist), col); 
	}
	void Add(Collider3D* col) {
		collisionlist.push_back(col); 
	}

	/* ˆÊ’uÝ’è*/
	void SetPosition(Vector3 pos) {
		m_position = pos; 
	}
	/* ˆÊ’uŽæ“¾*/
	Vector3 GetPosition() { 
		return gameObject.lock()->GetPosition() + m_position;
	}
	/* ƒTƒCƒYÝ’è */
	void SetSize(Vector3 size) {
		m_size = size; 
	}
	/* ƒTƒCƒYŽæ“¾ */
	Vector3 GetSize() { 
		return gameObject.lock()->GetScale() * m_size; 
	}
	/* ‰ñ“]ŠpÝ’è */
	void SetRotation(Vector3 rotation) {
		m_rotation = rotation;
	}
	/* ‰ñ“]ŠpŽæ“¾ */
	Vector3 GetRotation() {
		return gameObject.lock()->GetRotation() + m_rotation; 
	}

	ColliderType3D GetType() {
		return m_type; 
	};
	void SetLayer(Layer layer) {
		m_layer = layer;
	}
	bool ShouldJudgeCollide(Layer other) {
		return PhysicsLayer::CollisionTable[(int)m_layer][(int)other];
	}

	Vector3 Right() { 
		return Vector3(1, 0, 0) * Matrix4::RotateRollPitchYaw(GetRotation());
	}
	Vector3 Top() { 
		return Vector3(0, 1, 0) * Matrix4::RotateRollPitchYaw(GetRotation());
	}
	Vector3 Front() {
		return Vector3(0, 0, 1) * Matrix4::RotateRollPitchYaw(GetRotation()); 
	}

protected:
	std::vector<Collider3D*> collisionlist;
	std::vector<Collider3D*> precollisionlist;
	ColliderType3D m_type;
	Vector3 m_position;
	Vector3 m_size;
	Vector3 m_rotation;
	Layer m_layer;
};

