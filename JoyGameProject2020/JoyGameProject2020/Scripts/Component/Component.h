#pragma once
#include <memory>

class GameObject;
class Component {

public:
	virtual ~Component() {};
	/* ‰Šú‰» */
	virtual void Init() {};
	/* XV */
	virtual void Update() {};
	/* I—¹ */
	virtual void Shutdown() {};
	/* Õ“Ë‚µ‚½ */
	virtual void OnCollisionEnter(std::shared_ptr<GameObject> other) {};
	/* Õ“Ë‚µ‚Ä‚¢‚é */
	virtual void OnCollisionStay(std::shared_ptr<GameObject> other) {};
	/* Õ“Ë‚µ‚È‚­‚È‚Á‚½ */
	virtual void OnCollisionExit(std::shared_ptr<GameObject> other) {};

	std::weak_ptr<GameObject> gameObject;
};

