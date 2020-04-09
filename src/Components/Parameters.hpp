#pragma once

#include "Parameters/Parameter.hpp"
#include "Components/History.hpp"

#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace Cmp {
struct Parameters{

	//Parameters() = default;
	//Parameters(Parameters&&) noexcept = default;
	//Parameters(const Parameters&) = delete;
	//Parameters& operator=(const Parameters&) = delete;
	//Parameters& operator=(Parameters&&) noexcept = default;

	std::vector<std::shared_ptr<Parameter>> list; // We don't use unique_ptr because entt struggles with non-copyable objects (mainly the serialization part)
	Cmp::History history;

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