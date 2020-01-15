#include "LayersManager.hpp"

#include "Components/Translation.hpp"
#include "Components/Scale.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"

#include "App.hpp"

entt::entity LayersManager::addLayer() {
	entt::entity id = App::Get().m_registry.create();
	m_layersOrdered.push_back(id);

	App::Get().m_registry.assign<Cmp::Translation>(id, 0.0f, 0.0f);
	App::Get().m_registry.assign<Cmp::Scale>(id, 1.0f);
	App::Get().m_registry.assign<Cmp::AspectRatio>(id, 1.0f);
	App::Get().m_registry.assign<Cmp::Parent>(id, App::Get().m_drawingBoardId);

	return id;
}