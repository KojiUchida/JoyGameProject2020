#include "GameObject.h"
#include "Component/Component.h"

GameObject::GameObject() :
	m_isDestroy(false),
	m_isCollision(false),
	transform(),
	m_tag(""),
	billboard(Matrix4::Identity()) {
}

GameObject::~GameObject() {
}

void GameObject::Destroy() {
	m_isDestroy = true;
}

void GameObject::SetPosition(const Vector3& pos) {
	transform.position = pos;
}

Vector3 GameObject::GetPosition() {
	return transform.position;
}

void GameObject::SetScale(const Vector3& scale) {
	transform.scale= scale;
}

Vector3 GameObject::GetScale() {
	return transform.scale;
}

void GameObject::SetRotation(const Vector3& rotation) {
	transform.rotation = Quaternion::Euler(rotation);
}

Vector3 GameObject::GetRotation() {
	return transform.rotation.EulerAngles();
}

void GameObject::SetTag(const std::string& tag) {
	m_tag = tag;
}

std::string GameObject::GetTag() {
	return m_tag;
}

bool GameObject::CompareTag(const std::string& tag) {
	return m_tag == tag;
}

void GameObject::SetParent(const std::shared_ptr<GameObject> parent) {
	m_parent = parent;
	parent->m_children.push_back(shared_from_this());
}

std::shared_ptr<GameObject> GameObject::GetParent() {
	return m_parent.lock();
}

void GameObject::AddChild(const std::shared_ptr<GameObject> child) {
	m_children.push_back(child);
	child->m_parent = shared_from_this();
}

std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() {
	return m_children;
}

void GameObject::AddComponent(const std::shared_ptr<Component> component) {
	component->Init();
	m_components.push_back(component);
	component->gameObject = shared_from_this();
}

std::vector<std::shared_ptr<Component>> GameObject::GetComponents() {
	return m_components;
}
