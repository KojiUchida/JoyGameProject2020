#pragma once
#include <random>

class Random {
public:
	Random();
	~Random();

	/* 乱数のシード設定 */
	void Seed(const int seed);
	/* ただランダムな値が得られる */
	int GetRand();
	/* min～maxのランダムな値(int版) */
	int GetRand(const int min, const int max);
	/* min～maxのランダムな値(float版) */
	float GetRand(const float min, const float max);

private:
	std::mt19937 mt;
};

