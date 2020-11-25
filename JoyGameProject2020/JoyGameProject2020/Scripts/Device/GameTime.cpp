#include "GameTime.h"
#include <chrono>
#include <iostream>

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
	previousTime = startTime;
}

void GameTime::Update() {
	deltaTime = CurrentTime() - previousTime;
	previousTime = CurrentTime();

	static int count = 0;
	static float time = 0;
	time += DeltaTime();
	++count;
	if (time > 1) {
		std::cout << count << std::endl;
		time = 0;
		count = 0;
	}
}

double GameTime::CurrentTime() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000000.0;
}

double GameTime::ElapsedTime() {
	return CurrentTime() - startTime;
}

float GameTime::DeltaTime() {
	return (float)deltaTime;
}

