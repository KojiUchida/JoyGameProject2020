#include "Timer.h"
#include <chrono>
#include "Math/MathUtil.h"
#include "Device/GameTime.h"

Timer::Timer(float maxTime, bool loopFlag) :
	m_startTime(0.0f),
	m_elapsedTime(0.0f),
	m_maxTime(maxTime),
	m_isLoop(loopFlag),
	m_isUpdate(false),
	m_isTime(false) {
}

Timer::~Timer() {
}

void Timer::Update() {
	CheckLoop();

	if (!m_isUpdate)return;
	m_elapsedTime = (float)(GameTime::CurrentTime() - m_startTime);
}

void Timer::Start() {
	m_isUpdate = true;
	m_elapsedTime = 0.0f;
	m_startTime = (float)GameTime::CurrentTime();
}

void Timer::Stop() {
	m_isUpdate = false;
}

void Timer::Reset() {
	m_elapsedTime = 0.0f;
	m_startTime = (float)GameTime::CurrentTime();
}

void Timer::SetLimit(const float maxTime) {
	m_maxTime = maxTime;
}

void Timer::SetLoop(const bool loopFlag) {
	m_isLoop = loopFlag;
}

bool Timer::IsTime() {
	return m_elapsedTime >= m_maxTime;
}

float Timer::GetElapsed() {
	return m_elapsedTime;
}

float Timer::GetRatioElapsed() {
	if (m_maxTime == 0.0f) return 0.0f;
	return Math::Clamp(m_elapsedTime, 0.0f, m_maxTime) / m_maxTime;
}

float Timer::GetRemaining() {
	return m_maxTime - Math::Clamp(m_elapsedTime, 0.0f, m_maxTime);
}

float Timer::GetRatioRemaining() {
	return 1.0f - GetRatioElapsed();
}

void Timer::CheckLoop() {
	if (!m_isLoop) return;
	if (!IsTime()) return;
	Reset();
}

