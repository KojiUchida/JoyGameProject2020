#include "Timer.h"
#include <chrono>
#include "Math/MathUtility.h"
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
	if (!m_isUpdate)return;
	m_elapsedTime = (float)GameTime::CurrentTime() - m_startTime;

	if (!m_isLoop)return;
	if (m_elapsedTime <= m_maxTime)return;
	Reset();
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
	return m_isTime;
}

float Timer::GetElapsed() {
	return m_elapsedTime;
}

float Timer::GetRatioElapsed() {
	return MathUtility::Clamp(m_elapsedTime, 0, m_maxTime) / m_maxTime;
}

float Timer::GetRemaining() {
	return m_maxTime - MathUtility::Clamp(m_elapsedTime, 0, m_maxTime);
}

float Timer::GetRatioRemaining() {
	return 1.0f - GetRatioElapsed();
}

