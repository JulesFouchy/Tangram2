#pragma once

namespace Cmp {
struct Children {
	std::vector<entt::entity> list;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(list);
	}
};
}