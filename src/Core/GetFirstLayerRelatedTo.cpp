#include "GetFirstLayerRelatedTo.hpp"

#include "Components/Parent.hpp"

entt::entity TNG::GetFirstLayerRelatedTo(entt::registry& R, entt::entity e) {
	if (R.valid(e)) {
		if (R.has<entt::tag<"Layer"_hs>>(e))
			return e;
		else {
			Cmp::Parent* parent = R.try_get<Cmp::Parent>(e);
			if (parent)
				return GetFirstLayerRelatedTo(R, parent->id);
			else
				return entt::null;
		}
	}
	else
		return entt::null;
}