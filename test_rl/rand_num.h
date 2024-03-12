#pragma once
#include <vector>
#include <random>

class rand_num
{
public:
	rand_num();
	int get_rand(int num);
public:
	template<typename T>
	static int sampling(const T& probability);
	template<typename T>
	bool check_probability(const T& t);//检查概率和是否为1
};

template<typename T>
inline int rand_num::sampling(const T& probability)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dis(probability.begin(), probability.end());
	return dis(gen);
}

template<typename T>
inline bool rand_num::check_probability(const T& t)
{
	double sum = 0;
	for (auto& [k, v] : t)
		sum += v;
	return sum != 1.0;
}