#include "ShapeFactory.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

ShapeFactory::ShapeFactory(Instance& instance)
	: I(instance)
{}

entt::entity ShapeFactory::Point2D(glm::vec2 posInDrawingBoardSpace) {
	entt::registry& R = I.registry();
	auto id = R.create();
	R.assign<entt::tag<"Point2D"_hs>>(id);
	glm::mat3 transformMatrix = glm::translate(glm::mat3(1.0f), posInDrawingBoardSpace);
	transformMatrix = glm::scale(transformMatrix, glm::vec2(0.05f));
	R.assign<Cmp::TransformMatrix>(id, Cmp::TransformMatrix(transformMatrix));
	R.assign<Cmp::Parent>(id, I.drawingBoardId());
	return id;
}