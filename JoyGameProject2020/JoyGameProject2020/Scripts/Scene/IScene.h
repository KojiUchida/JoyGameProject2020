#pragma once
#include <string>

class IScene {
public:
	virtual ~IScene() {};
	/* ������ */
	virtual void Init() = 0;
	/* �X�V */
	virtual void Update() = 0;
	/* �I�� */
	virtual void Shutdown() = 0;
	/* ���̃V�[�� */
	virtual std::string NextScene() = 0;
	/* �I������ */
	virtual bool IsEnd() = 0;
};

