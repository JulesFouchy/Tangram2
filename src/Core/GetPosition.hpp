#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Components/TransformMatrix.hpp"

namespace TNG {
	inline glm::vec2 GetPosition(entt::registry& R, entt::entity e) {
		const glm::mat3& mat = R.get<Cmp::TransformMatrix>(e).val();
		return glm::vec2(mat[2][0], mat[2][1]) / mat[2][2];
	}
}