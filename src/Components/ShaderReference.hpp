#pragma once

#include <entt/entt.hpp>

#include <cereal/access.hpp>

namespace Cmp {
struct ShaderReference {
	entt::entity entityID;

	ShaderReference() = default;

	ShaderReference(entt::entity e)
		: entityID(e) {}

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(entityID);
	}
};
}