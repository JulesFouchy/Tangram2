#pragma once

namespace TNG {
	inline void MustRecomputeTexture(entt::registry& R, entt::entity layer){
		R.assign_or_replace<entt::tag<"MustRecomputeTexture"_hs>>(layer);
	}
}