#pragma once

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