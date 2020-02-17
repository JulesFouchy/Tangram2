#pragma once

#include <string>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

namespace Cmp {
struct SliderFloat {
	std::string name;
	float val;
	float minVal;
	float maxVal;
	std::string format;
	float power;

	SliderFloat(const std::string& name, float defaultValue, float minValue, float maxValue, const std::string& format = "%.3f", float power = 1.0f)
		: name(name), val(defaultValue), minVal(minValue), maxVal(maxValue), format(format), power(power)
	{}

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(name, val, minVal, maxVal, format, power);
	}
};
}