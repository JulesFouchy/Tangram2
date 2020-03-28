#pragma once

#include <string>

#include <glm/glm.hpp>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

namespace Cmp {
	struct SliderFloat2 {
		std::string name;
		glm::vec2 val;
		float minVal;
		float maxVal;
		std::string format;
		float power;

		SliderFloat2(const std::string& name, const glm::vec2& defaultValue, float minValue, float maxValue, const std::string& format = "%.3f", float power = 1.0f)
			: name(name), val(defaultValue), minVal(minValue), maxVal(maxValue), format(format), power(power)
		{}
		SliderFloat2() = default;

	private:
		//Serialization
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(name, val.x, val.y, minVal, maxVal, format, power);
		}
	};
}