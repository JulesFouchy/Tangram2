#pragma once

#include "Parameters/Parameter.hpp"
#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace Cmp {
struct Parameters{

	Parameters() = default;
	Parameters(Parameters&&) noexcept = default;
	Parameters(const Parameters&) = delete;
	Parameters& operator=(const Parameters&) = delete;
	Parameters& operator=(Parameters&&) noexcept = default;

	std::vector<std::unique_ptr<Parameter>> list;

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