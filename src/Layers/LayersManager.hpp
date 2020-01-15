#pragma once

#include <vector>

#include <entt/entt.hpp>

#include <glm/glm.hpp>

class LayersManager {
friend class RenderSystem;
public:
	LayersManager() = default;
	~LayersManager() = default;

	entt::entity addLayer();

public:
	entt::entity hoveredLayer(const glm::vec2& posInNDC);

private:
	std::vector<entt::entity> m_layersOrdered;
};