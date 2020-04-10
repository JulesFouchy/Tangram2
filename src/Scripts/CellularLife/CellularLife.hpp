#pragma once

#include "Scripts/Script.hpp"

#include "Parameters/ConcreteParameters.hpp"
#include "Helper/Random.hpp"

#include "Cell.hpp"
#include "CellTypes.hpp"

class LayersManager;

class CellularLife : Script {
public:
	CellularLife() = default;
	CellularLife(entt::registry& R, LayersManager& layersM);
	~CellularLife() = default;

	void loopIteration(entt::registry& R, float dt) override;
	void ImGui(entt::registry& R) override;

private:
	void applyInteractions(entt::registry& R, float dt);
	glm::vec2 computeForce(glm::vec2 p1, glm::vec2 p2, unsigned int id1, unsigned int id2);

	void randomizeSettings();

	std::vector<Point2DParameter>& getPointsList(entt::registry& R);
	void resetPositions(entt::registry& R);
	void checkEntityValidity(entt::registry& R);

private:
	Rand m_rand;
	entt::entity m_layer;
	std::vector<Cell> m_cells;

	float m_dampingCoef;
	float m_maxRadius;
	ListOfSettings m_settings;
};