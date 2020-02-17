#pragma once

#include <string>

#include <entt/entt.hpp>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace Cmp {
struct ColorPicker3 {
	std::string name;
	glm::vec3 val;
	ImGuiColorEditFlags flags;

	ColorPicker3(const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags = 0)
		: name(name), val(val), flags(flags)
	{}
	ColorPicker3() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			name, 
			val.x, val.y, val.z,
			flags
		);
	}
};
}