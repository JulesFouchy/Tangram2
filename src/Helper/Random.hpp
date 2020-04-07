#pragma once

#include <random>

class Rand {
public:
	Rand();
	Rand(int seed);

	float Float();

private:
	static std::random_device trueRandomGenerator;
	std::default_random_engine m_generator;
	std::uniform_real_distribution<float> m_distributionFloat;
};