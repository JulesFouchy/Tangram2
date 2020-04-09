#pragma once

#include "Components/Parent.hpp"
#include "Components/Children.hpp"

namespace TNG {
	inline void CreateParentRelationship(entt::registry& R, entt::entity child, entt::entity parent) {
		R.get<Cmp::Parent>(child).id = parent;
		R.get<Cmp::Children>(parent).list.push_back(child);
	}
}