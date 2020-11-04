#pragma once
#include <random>

class Random {
public:
	Random();
	~Random();

	/* �����̃V�[�h�ݒ� */
	void Seed(const int seed);
	/* ���������_���Ȓl�������� */
	int GetRand();
	/* min�`max�̃����_���Ȓl(int��) */
	int GetRand(const int min, const int max);
	/* min�`max�̃����_���Ȓl(float��) */
	float GetRand(const float min, const float max);

private:
	std::mt19937 mt;
};

