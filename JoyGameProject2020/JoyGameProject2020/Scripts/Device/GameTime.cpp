#include "GameTime.h"
#include <chrono>

static double startTime;
static double previousTime;
static double deltaTime;

GameTime::GameTime() {
}

GameTime::~GameTime() {
}

GameTime& GameTime::Instance() {
	static GameTime instance;
	return instance;
}

void GameTime::Init() {
	startTime = CurrentTime();
}

void GameTime::Update() {
	deltaTime = CurrentTime() - previousTime;
	previousTime = CurrentTime();
}

double GameTime::CurrentTime() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000000.0;
}

double GameTime::ElapsedTime() {
	return CurrentTime() - startTime;
}

double GameTime::DeltaTime() {
	return deltaTime;
}

