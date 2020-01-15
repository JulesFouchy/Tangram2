#pragma once

#include <vector>

#include <entt/entt.hpp>

class LayersManager {
friend class RenderSystem;
public:
	LayersManager() = default;
	~LayersManager() = default;

	entt::entity addLayer();

private:
	std::vector<entt::entity> m_layersOrdered;
};