#pragma once

#include <vector>

#include <entt/entt.hpp>

#include <glm/glm.hpp>

#include "System/ISystem.hpp"

class LayersManager : public ISystem {
friend class RenderSystem;
public:
	LayersManager(Instance& instance);
	~LayersManager() = default;

	entt::entity addLayer();

public:
	entt::entity layerHoveredBy(const glm::vec2& posInNDC);
	entt::entity layerHoveredByMouse();

private:
	std::vector<entt::entity> m_layersOrdered;
};