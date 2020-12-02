#pragma once
#include <vector>

class Ranking
{
public:
	Ranking() {};
	Ranking(const Ranking&) {};
	Ranking&operator=(const Ranking&) {};
	static Ranking& Instance();
	~Ranking();

	void ResetRanking();
	void SetRanking(const double& time);
	unsigned int GetRanking(const double& time);
	double GetRanking(const unsigned int& rank);
	bool IsRank(const double& time);

	//↓デバッグ用表示関数
	void GetAllRanking();
	void GetMyRanking(const double& time);

private:
	std::vector<double>times;
};