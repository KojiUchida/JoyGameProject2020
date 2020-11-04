#pragma once
#include <memory>

class GameObject;
class Component {

public:
	virtual ~Component() {};
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

	std::weak_ptr<GameObject> gameObject;
};

