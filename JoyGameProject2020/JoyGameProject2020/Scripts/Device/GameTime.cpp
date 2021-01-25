#include "GameTime.h"
#include <chrono>

static double startTime;
static double previousTime;
static double deltaTime;
static float timeScale;
static double baseTime;
static double fps;
static int count;

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
	timeScale = 1.0f;
	fps = 0;
	count = 0;
}

void GameTime::Update() {
	deltaTime = CurrentTime() - previousTime;
	previousTime = CurrentTime();

	++count;
	double timeDiff = CurrentTime() - baseTime;
	if (timeDiff > 1) {
		fps = (double)count / timeDiff;
		count = 0;
		baseTime = CurrentTime();
	}
}

double GameTime::CurrentTime() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000000.0;
}

double GameTime::ElapsedTime() {
	return CurrentTime() - startTime;
}

double GameTime::DeltaTime() {
	return deltaTime * timeScale;
}

double GameTime::FPS() {
	return fps;
}

double GameTime::UnscaledDeltaTime() {
	return deltaTime;
}

