#include "ShapeFactory.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/VisualDependencies.hpp"

#include "Core/CreateParentRelationship.hpp"

entt::entity ShapeFactory::CreatePoint2D(entt::registry& R, entt::entity parent, glm::vec2 posInDrawingBoardSpace) {
	entt::entity e = R.create();
	R.assign<entt::tag<"Point2D"_hs>>(e);
	glm::mat3 transformMatrix = glm::translate(glm::mat3(1.0f), posInDrawingBoardSpace);
	transformMatrix = glm::scale(transformMatrix, glm::vec2(0.05f));
	R.assign<Cmp::TransformMatrix>(e, Cmp::TransformMatrix(transformMatrix));
	R.assign<Cmp::Parent>(e, entt::null);
	R.assign<Cmp::VisualDependencies>(e).list.push_back(parent);
	TNG::CreateParentRelationship(R, e, parent);
	R.assign<Cmp::Children>(e);
	return e;
}