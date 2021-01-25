#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Math/Transform.h"
#include "Math/Matrix4.h"

class Component;
class GameObjectManager;
class GameObject : public std::enable_shared_from_this<GameObject> {
	friend GameObjectManager;

public:
	GameObject();
	virtual ~GameObject();

	/* ������ */
	virtual void Init() {};
	/* �X�V */
	virtual void Update() {};
	/* �I�� */
	virtual void Shutdown() {};
	/* �Փ˂��� */
	virtual void OnCollisionEnter(std::shared_ptr<GameObject> other) {};
	/* �Փ˂��Ă��� */
	virtual void OnCollisionStay(std::shared_ptr<GameObject> other) {};
	/* �Փ˂��Ȃ��Ȃ��� */
	virtual void OnCollisionExit(std::shared_ptr<GameObject> other) {};

	/* �j�� */
	void Destroy();

	/* �ʒu�ݒ� */
	void SetPosition(const Vector3& pos);
	/* �ʒu�擾 */
	Vector3 GetPosition();

	/* �T�C�Y�ݒ� */
	void SetScale(const Vector3& scale);
	/* �T�C�Y�擾 */
	Vector3 GetScale();

	/* ��]�ݒ� */
	void SetRotation(const Vector3& rotation);
	/* ��]�擾 */
	Vector3 GetRotation();

	Vector3 Right();
	Vector3 Up();
	Vector3 Front();

	/* �^�O�ݒ� */
	void SetTag(const std::string& tag);
	/* �^�O�擾 */
	std::string GetTag();
	/* �^�O�̔�r */
	bool CompareTag(const std::string& tag);

	/* �e�̐ݒ� */
	void SetParent(const std::shared_ptr<GameObject> parent);
	/* �e�̎擾 */
	std::shared_ptr<GameObject> GetParent();

	/* �q�̒ǉ� */
	void AddChild(const std::shared_ptr<GameObject> child);
	/* �q�̎擾 */
	std::vector<std::shared_ptr<GameObject>> GetChildren();

	/* �R���|�[�l���g�̒ǉ� */
	void AddComponent(const std::shared_ptr<Component> component);
	/* �R���|�[�l���g�̎擾 */
	std::vector<std::shared_ptr<Component>> GetComponents();

	Transform transform;
	Matrix4 billboard;

protected:
	std::weak_ptr<GameObject> m_parent;
	std::vector<std::shared_ptr<GameObject>> m_children;
	std::vector<std::shared_ptr<Component>> m_components;
	bool m_isDestroy;
	bool m_isCollision;
	std::string m_tag;
};

