#pragma once

#include <entt/entt.hpp>

class LayersManager;

class CellularLife {
public:
	CellularLife() = default;
	CellularLife(LayersManager& layersM);
	~CellularLife() = default;

	void loopIteration(float dt, entt::registry& R);

private:
	entt::entity m_layer;
};