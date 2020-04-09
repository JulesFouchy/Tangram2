#pragma once

class ShapeFactory {
public:
	ShapeFactory() = delete;

	static entt::entity CreatePoint2D(entt::registry& R, entt::entity parent, glm::vec2 posInDrawingBoardSpace);
};