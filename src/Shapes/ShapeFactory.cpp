#include "ShapeFactory.hpp"

#include "Instance.hpp"

ShapeFactory::ShapeFactory(Instance& instance)
	: I(instance)
{}

entt::entity ShapeFactory::Point2D(glm::vec2 posInScreenSpace) {
	entt::registry& R = I.registry();
	auto id = R.create();
	R.assign<glm::vec2>(id, posInScreenSpace);
	return id;
}