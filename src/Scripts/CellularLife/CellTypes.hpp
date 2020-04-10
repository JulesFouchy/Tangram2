#pragma once

#include <array>
#define NB_TYPES 2

struct InteractionSettings {
	float attractionStrengthMax;
	float attractionDistanceMax;
	float repulsionStrengthMax;
	float repulsionDistanceMax;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(attractionStrengthMax), 
			CEREAL_NVP(attractionDistanceMax),
			CEREAL_NVP(repulsionStrengthMax),
			CEREAL_NVP(repulsionDistanceMax)
		);
	}
};

struct ListOfSettings {
public:
	std::array<InteractionSettings, NB_TYPES>& operator [](size_t index) { return m_list[index]; }
private:
	std::array<std::array<InteractionSettings, NB_TYPES>, NB_TYPES> m_list;
};