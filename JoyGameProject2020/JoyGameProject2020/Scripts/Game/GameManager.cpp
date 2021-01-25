#include "GameManager.h"
#include "Utility/Timer.h"

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

bool GameManager::CompareState(GameState state) {
	return state == m_state;
}

void GameManager::ChangeState(GameState state) {
	m_state = state;
	OnChangeState();
}

void GameManager::OnChangeState() {
	m_timer->Reset();
}
