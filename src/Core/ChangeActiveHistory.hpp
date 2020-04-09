#pragma once

namespace TNG {
	inline void SetActiveHistoryToParameters(entt::registry& R, entt::entity layer) {
		R.assign_or_replace<entt::tag<"ActiveHistoryIsParameter"_hs>>(layer);
	}

	inline void SetActiveHistoryToTransform(entt::registry& R, entt::entity layer) {
		R.reset<entt::tag<"ActiveHistoryIsParameter"_hs>>(layer);
	}
}