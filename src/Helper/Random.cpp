#include "Random.hpp"

#include "Debugging/Log.hpp"

std::random_device Rand::trueRandomGenerator;

Rand::Rand()
	: Rand(trueRandomGenerator())
{}

Rand::Rand(int seed)
	: m_generator(seed), m_distributionFloat(0.0f, 1.0f)
{
	spdlog::info("[Rand::Rand] using seed : {}", seed);
}

float Rand::Float() {
	return m_distributionFloat(m_generator);
}