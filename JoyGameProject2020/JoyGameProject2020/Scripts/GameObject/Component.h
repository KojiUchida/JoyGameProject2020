#pragma once
#include <memory>

class GameObject;
class Component {

public:
	virtual ~Component() {};
	/* 初期化 */
	virtual void Init() {};
	/* 更新 */
	virtual void Update() {};
	/* 終了 */
	virtual void Shutdown() {};
	/* 衝突した */
	virtual void OnCollisionEnter(std::shared_ptr<GameObject> other) {};
	/* 衝突している */
	virtual void OnCollisionStay(std::shared_ptr<GameObject> other) {};
	/* 衝突しなくなった */
	virtual void OnCollisionExit(std::shared_ptr<GameObject> other) {};

	std::weak_ptr<GameObject> gameObject;
};

