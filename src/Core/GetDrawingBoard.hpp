#pragma once

namespace TNG {
	inline entt::entity GetDrawingBoard(entt::registry& R) {
		return R.view<entt::tag<"DrawingBoard"_hs>>()[0];
	}
}