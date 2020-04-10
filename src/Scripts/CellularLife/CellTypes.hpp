#pragma once

#include <array>
#define NB_TYPES 2

struct InteractionSettings {
	float attractionStrengthMax;
	float attractionDistanceMax;
	float repulsionStrengthMax;
	float repulsionDistanceMax;
};

struct ListOfSettings {
public:
	std::array<InteractionSettings, NB_TYPES>& operator [](size_t index) { return m_list[index]; }
private:
	std::array<std::array<InteractionSettings, NB_TYPES>, NB_TYPES> m_list;
};