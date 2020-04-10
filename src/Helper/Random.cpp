#include "Random.hpp"

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
float Rand::Float(float max) {
	return Float() * max;
}
float Rand::Float(float min, float max) {
	return Float(max - min) + min;
}

int Rand::Int(int maxExcluded) {
	return floor(Float() * maxExcluded);
}
int Rand::Int(int min, int maxExcluded) {
	return floor(Float() * (maxExcluded - min) + min);
}