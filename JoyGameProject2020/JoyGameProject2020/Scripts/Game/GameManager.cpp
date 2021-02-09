#include "GameManager.h"
#include "Utility/Timer.h"
#include "Game/Ranking.h"

GameManager::GameManager() :
	m_state(GameState::NONE) {
	m_timer = std::make_shared<Timer>();
	m_timer->Start();
}

GameManager::~GameManager() {
}

GameManager& GameManager::Instance() {
	static GameManager instance;
	return instance;
}

void GameManager::Update() {
	m_timer->Update();
}

GameState GameManager::GetState() {
	return m_state;
}

bool GameManager::CompareState(GameState state) {
	return state == m_state;
}

void GameManager::ChangeState(GameState state) {
	m_state = state;
	if (state == GameState::GOAL)
	{
		cleartime = TimeElapsedOnCurrentState();
		Ranking::Instance().SetRanking(cleartime);
	}
		OnChangeState();
}

void GameManager::OnChangeState() {
	m_timer->Reset();
}

float GameManager::TimeElapsedOnCurrentState() {
	return m_timer->GetElapsed();
}

float GameManager::GetClearTime()
{
	return cleartime;
}
