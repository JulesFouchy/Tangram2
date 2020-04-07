#pragma once

#include <entt/entt.hpp>

#include "Parameters/ConcreteParameters.hpp"
#include "Helper/Random.hpp"

#include "Cell.hpp"

class LayersManager;

class CellularLife {
public:
	CellularLife() = default;
	CellularLife(entt::registry& R, LayersManager& layersM);
	~CellularLife() = default;

	void loopIteration(float dt, entt::registry& R);

	void ImGui(entt::registry& R);

private:
	std::vector<Point2DParameter>& getPointsList(entt::registry& R);
	void resetPositions(entt::registry& R);

private:
	Rand m_rand;
	entt::entity m_layer;
	std::vector<Cell> m_cells;

	float m_dampingCoef;
};