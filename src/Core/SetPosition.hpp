#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Components/TransformMatrix.hpp"

namespace TNG {
	inline void SetPosition(entt::registry& R, entt::entity e, const glm::vec2& newPos) {
		glm::mat3 mat = R.get<Cmp::TransformMatrix>(e).val();
		mat[2][0] = newPos.x;
		mat[2][1] = newPos.y;
		mat[2][2] = 1.0f;
		R.replace<Cmp::TransformMatrix>(e, mat);
	}
}