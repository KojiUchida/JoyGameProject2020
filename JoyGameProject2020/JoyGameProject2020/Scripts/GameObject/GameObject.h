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

	/* 破棄 */
	void Destroy();

	/* 位置設定 */
	void SetPosition(const Vector3& pos);
	/* 位置取得 */
	Vector3 GetPosition();

	/* サイズ設定 */
	void SetScale(const Vector3& scale);
	/* サイズ取得 */
	Vector3 GetScale();

	/* 回転設定 */
	void SetRotation(const Vector3& rotation);
	/* 回転取得 */
	Vector3 GetRotation();

	Vector3 Right();
	Vector3 Up();
	Vector3 Front();

	/* タグ設定 */
	void SetTag(const std::string& tag);
	/* タグ取得 */
	std::string GetTag();
	/* タグの比較 */
	bool CompareTag(const std::string& tag);

	/* 親の設定 */
	void SetParent(const std::shared_ptr<GameObject> parent);
	/* 親の取得 */
	std::shared_ptr<GameObject> GetParent();

	/* 子の追加 */
	void AddChild(const std::shared_ptr<GameObject> child);
	/* 子の取得 */
	std::vector<std::shared_ptr<GameObject>> GetChildren();

	/* コンポーネントの追加 */
	void AddComponent(const std::shared_ptr<Component> component);
	/* コンポーネントの取得 */
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

