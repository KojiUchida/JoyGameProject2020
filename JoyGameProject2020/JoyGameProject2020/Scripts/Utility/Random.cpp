#include "Random.h"

Random::Random() {
	mt = std::mt19937(std::random_device{}());
}

Random::~Random() {
}

void Random::Seed(const int seed) {
	mt.seed(seed);
}

int Random::GetRand() {
	return mt();
}

int Random::GetRand(const int min, const  int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

float Random::GetRand(const float min, const float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}
