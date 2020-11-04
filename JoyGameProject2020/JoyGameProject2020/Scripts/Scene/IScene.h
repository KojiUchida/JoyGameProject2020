#pragma once
#include <string>

class IScene {
public:
	virtual ~IScene() {};
	/* 初期化 */
	virtual void Init() = 0;
	/* 更新 */
	virtual void Update() = 0;
	/* 終了 */
	virtual void Shutdown() = 0;
	/* 次のシーン */
	virtual std::string NextScene() = 0;
	/* 終了条件 */
	virtual bool IsEnd() = 0;
};

