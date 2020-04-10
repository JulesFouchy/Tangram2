#pragma once

#include <random>

class Rand {
public:
	Rand();
	Rand(int seed);

	float Float();
	float Float(float max);
	float Float(float min, float max);
	int Int(int maxExcluded);
	int Int(int min, int maxExcluded);

private:
	static std::random_device trueRandomGenerator;
	std::default_random_engine m_generator;
	std::uniform_real_distribution<float> m_distributionFloat;
};