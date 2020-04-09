#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace TNG {
	glm::mat3 GetLocalTransform(entt::registry& R, entt::entity e);
	glm::mat3 GetMatrixPlusAspectRatio(entt::registry& R, entt::entity e);
	glm::mat3 GetMatrix(entt::registry& R, entt::entity e);
	glm::mat3 GetMatrixToDBSpace(entt::registry& R, entt::entity e);
	glm::mat3 GetMatrixToTextureSpace(entt::registry& R, entt::entity e);
	glm::mat3 GetParentModelMatrix(entt::registry& R, entt::entity e);
	glm::mat3 GetParentModelMatrixExcludingDB(entt::registry& R, entt::entity e);
}