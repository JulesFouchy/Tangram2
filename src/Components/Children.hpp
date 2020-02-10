#pragma once

#include <entt/entt.hpp>
#include <cereal/access.hpp>

namespace Cmp {
struct Children {
	std::vector<entt::entity> list;

};
}