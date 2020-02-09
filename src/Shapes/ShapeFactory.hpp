#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>

class Instance;

class ShapeFactory {
public:
	ShapeFactory(Instance& instance);
	~ShapeFactory() = default;

	entt::entity createPoint2D(glm::vec2 posInScreenSpace, entt::entity parent) const;
	entt::entity createPolygon(const std::vector<glm::vec2>& vertices) const;
private:
	Instance& I;
};