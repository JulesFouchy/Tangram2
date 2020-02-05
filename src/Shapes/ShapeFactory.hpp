#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

class Instance;

class ShapeFactory {
public:
	ShapeFactory(Instance& instance);
	~ShapeFactory() = default;

	entt::entity Point2D(glm::vec2 posInScreenSpace);
private:
	Instance& I;
};