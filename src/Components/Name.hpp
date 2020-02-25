#pragma once

#include <string>

#include <entt/entt.hpp>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

namespace Cmp {
struct Name {
	std::string val;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(val);
	}
};
}