#include "LayersManager.hpp"

#include "Components/Translation.hpp"
#include "Components/Scale.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"

#include "App.hpp"

#include "Debugging/Log.hpp"

entt::entity LayersManager::addLayer() {
	entt::entity id = App::Get().m_registry.create();
	m_layersOrdered.push_back(id);

	App::Get().m_registry.assign<Cmp::Translation>(id, 0.0f, 0.0f);
	App::Get().m_registry.assign<Cmp::Scale>(id, 1.0f);
	App::Get().m_registry.assign<Cmp::AspectRatio>(id, 1.0f);
	App::Get().m_registry.assign<Cmp::Parent>(id, App::Get().m_drawingBoardId);

	return id;
}

entt::entity LayersManager::hoveredLayer(const glm::vec2& posInNDC) {
	for (auto it = m_layersOrdered.crbegin(); it < m_layersOrdered.crend(); it++) {
		glm::mat3 mat = App::Get().m_renderSystem.getMatrix(*it);
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		if (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f) {
			spdlog::info((int)*it);
			return *it;
		}
	}
	return entt::null;
}