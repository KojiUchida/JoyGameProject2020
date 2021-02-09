#include "Ranking.h"
#include <memory>
#include <algorithm>
#include <iostream>

Ranking & Ranking::Instance()
{
	static std::unique_ptr<Ranking>instance(new Ranking());
	return *instance;
}

Ranking::~Ranking()
{
	times.clear();
}

void Ranking::Initialize()
{
	//times.resize(10)
}

void Ranking::ResetRanking()
{
	times.clear();
}

void Ranking::SetRanking(const double& time)
{
	auto isSameTime = std::any_of(times.begin(), times.end(), [&](double value) {return value == time; });
	if (isSameTime)
	{
		return;//‚¢‚¸‚ê‚©‚É“¯‚¶time‚ª‚ ‚ê‚Î“o˜^‚È‚µ
	}

	//10ˆÊŒ—ŠO‚È‚ç11‚É“ü‚ê‚é
	auto itr = std::find_if(times.begin(), times.end(), [&](double value)
	{
		return 1 - (value < time);
	});

	times.emplace(itr, time);

	if (times.size() >= 11)
	{
		times.pop_back();
	}
}


unsigned int Ranking::GetRanking(const double& time)
{
	auto itr = std::find(times.begin(), times.end(), time);
	return itr - times.begin() + 1;
}

double Ranking::GetRankingTime(const unsigned int & rank)
{
	if (times.size() < rank)
	{
		return 0;
	}
	return times[rank-1];
}

bool Ranking::IsRank(const double & time)
{
	if (times.size() < 11)
	{
		return true;
	}
	else
	{
		if (times[9] > time)return true;
	}
	return false;
}

void Ranking::GetAllRanking()
{
	for (int i = 0; i < times.size();)
	{

		std::cout << times[i] << "_Rank " << GetRanking(times[i]) << std::endl;
		++i;
	}
	std::cout << std::endl;
}

void Ranking::GetMyRanking(const double& time)
{
	auto isNotRank = std::any_of(times.begin(), times.end(), [&](double value)
	{
		return value < time;
	});
	if (isNotRank&&times.size() >= 12)
	{
		std::cout << "MyTime:" << time << "_Rank " << "Œ—ŠO" << std::endl;
	}
	else
	{
		auto itr = std::find(times.begin(), times.end(), time);
		std::cout << "MyTime:" << time << "_Rank " << itr - times.begin() + 1 << std::endl;
	}
}
