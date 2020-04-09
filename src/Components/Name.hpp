#pragma once

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