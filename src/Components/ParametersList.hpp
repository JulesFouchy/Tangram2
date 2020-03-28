#pragma once

#include "Parameters/Parameter.hpp"
#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

namespace Cmp {
struct Parameters {
	std::vector<Parameter*> list;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		//archive(list);
	}
};
}