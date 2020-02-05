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

entt::entity LayersManager::layerHoveredBy(const glm::vec2& posInNDC) {
	for (auto it = m_layersOrdered.crbegin(); it < m_layersOrdered.crend(); it++) {
		glm::mat3 mat = I.getMatrixPlusAspectRatio(*it);
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		if (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f) {
			return *it;
		}
	}
	return entt::null;
}

entt::entity LayersManager::layerHoveredByMouse() {
	return layerHoveredBy(DisplayInfos::MousePositionInNormalizedDeviceCoordinates());
}