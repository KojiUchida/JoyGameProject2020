#pragma once
#include <memory>

enum class GameState {
	NONE,
	READY,
	PLAY,
	GOAL,
	GAMEOVER,
};

class Timer;
class GameManager {
private:
	GameManager();
	~GameManager();
	GameManager(const GameManager&) = delete;
	void operator= (const GameManager&) = delete;

public:
	static GameManager& Instance();

	void Update();
	GameState GetState();
	bool CompareState(GameState state);
	void ChangeState(GameState state);
	float TimeElapsedOnCurrentState();

	float GetClearTime();

private:
	void OnChangeState();

private:
	GameState m_state;
	std::shared_ptr<Timer> m_timer;
	double cleartime;
};

