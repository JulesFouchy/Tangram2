#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

class ShapeFactory {
public:
	ShapeFactory() = delete;

	static entt::entity CreatePoint2D(entt::registry& R, entt::entity parent, glm::vec2 posInDrawingBoardSpace);
};