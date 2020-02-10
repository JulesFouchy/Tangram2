#pragma once

#include <entt/entt.hpp>
#include <cereal/access.hpp>

namespace Cmp {
struct VisualDependencies {
	std::vector<entt::entity> list;
};
}