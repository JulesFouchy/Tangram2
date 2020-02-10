#include "ShapeFactory.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/VisualDependencies.hpp"
#include "Components/Vertices.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

ShapeFactory::ShapeFactory(Instance& instance)
	: I(instance)
{}

entt::entity ShapeFactory::createPoint2D(glm::vec2 posInDrawingBoardSpace, entt::entity parent) const {
	entt::registry& R = I.registry();
	entt::entity e = R.create();
	R.assign<entt::tag<"Point2D"_hs>>(e);
	glm::mat3 transformMatrix = glm::translate(glm::mat3(1.0f), posInDrawingBoardSpace);
	transformMatrix = glm::scale(transformMatrix, glm::vec2(0.018f));
	R.assign<Cmp::TransformMatrix>(e, Cmp::TransformMatrix(transformMatrix));
	R.assign<Cmp::Parent>(e, entt::null);
	R.assign<Cmp::VisualDependencies>(e).list.push_back(parent);
	I.setParentOf(e, parent);
	R.assign<Cmp::Children>(e);
	return e;
}