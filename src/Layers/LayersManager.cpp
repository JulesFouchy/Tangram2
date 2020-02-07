#include "LayersManager.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Debugging/Log.hpp"

LayersManager::LayersManager(Instance& instance)
	: ISystem(instance)
{}

entt::entity LayersManager::addLayer() {
	entt::entity id = I.registry().create();
	m_layersOrdered.push_back(id);

	I.registry().assign<Cmp::TransformMatrix>(id);
	I.registry().assign<Cmp::AspectRatio>(id, 1.0f);
	I.registry().assign<Cmp::Parent>(id, I.drawingBoardId());

	return id;
}

entt::entity LayersManager::getEntityHoveredBy(const glm::vec2& posInNDC) {
	// Check Points2D	
	auto points2D = I.registry().view<entt::tag<"Point2D"_hs>>();
	for (auto entity : points2D) {
		if (isEntityHoveredBy(entity, posInNDC))
			return entity;
	}
	// Check layers
	for (auto it = m_layersOrdered.crbegin(); it < m_layersOrdered.crend(); it++) {
		if (isEntityHoveredBy(*it, posInNDC))
			return *it;
	}
	return entt::null;
}

entt::entity LayersManager::getEntityHoveredByMouse() {
	return getEntityHoveredBy(DisplayInfos::MousePositionInNormalizedDeviceCoordinates());
}

bool LayersManager::isEntityHoveredBy(entt::entity e, const glm::vec2& posInNDC) {
	glm::mat3 mat = I.getMatrixPlusAspectRatio(e);
	glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
	return (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f);
}