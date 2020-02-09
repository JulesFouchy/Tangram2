#include "ShapeFactory.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/Vertices.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

ShapeFactory::ShapeFactory(Instance& instance)
	: I(instance)
{}

entt::entity ShapeFactory::createPoint2D(glm::vec2 posInDrawingBoardSpace, entt::entity parent) const {
	entt::registry& R = I.registry();
	entt::entity id = R.create();
	R.assign<entt::tag<"Point2D"_hs>>(id);
	glm::mat3 transformMatrix = glm::translate(glm::mat3(1.0f), posInDrawingBoardSpace);
	transformMatrix = glm::scale(transformMatrix, glm::vec2(0.018f));
	R.assign<Cmp::TransformMatrix>(id, Cmp::TransformMatrix(transformMatrix));
	R.assign<Cmp::Parent>(id, parent);
	return id;
}

entt::entity ShapeFactory::createPolygon(const std::vector<glm::vec2>& vertices) const {
	entt::registry& R = I.registry();
	entt::entity id = R.create();
	R.assign<entt::tag<"Polygon"_hs>>(id);
	R.assign<Cmp::Vertices>(id, vertices, I.drawingBoardId(), *this);
	return id;
}