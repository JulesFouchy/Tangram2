#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

#include "Components/Parameters.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/Random.hpp"

#include <imgui/imgui.h>

CellularLife::CellularLife(entt::registry& R, LayersManager& layersM)
	: m_dampingCoef(8.0f), m_attraction(4.42f), m_repulsionMargin(0.674f), m_maxRadius(0.341f)
{
	m_layer = layersM.createFragmentLayer(R, "res/shaders/testCellDistortion.frag");
	std::vector<Point2DParameter>& pts = getPointsList(R);
	for (Point2DParameter& pt : pts) {
		entt::entity e = pt.getEntity();
		m_cells.emplace_back(e);
	}
	resetPositions(R);
}

void CellularLife::resetPositions(entt::registry& R) {
	float ratio = DisplayInfos::Ratio();
	for (Cell& cell : m_cells)
		cell.setPosition(R, glm::vec2((m_rand.Float()-0.5f)*ratio, m_rand.Float()-0.5f));
}

void CellularLife::loopIteration(entt::registry& R, float dt) {
	checkEntityValidity(R);
	applyInteractions(R, dt);
	for (Cell& cell : m_cells) {
		cell.applyDamping(dt, m_dampingCoef);
		cell.move(R, dt, m_maxRadius);
	}
}

void CellularLife::applyInteractions(entt::registry& R, float dt) {
	for (size_t i = 0; i < m_cells.size(); ++i) {
		for (size_t j = i + 1; j < m_cells.size(); ++j) {
			glm::vec2 p1 = m_cells[i].getPosition(R);
			glm::vec2 p2 = m_cells[j].getPosition(R);
			float d = glm::distance(p1, p2);
			float m = m_repulsionMargin;
			float force = 1 / pow(d + m, 2.0f) - 1 / pow(d + m, 4.0f);
			force *= m_attraction;
			glm::vec2 dir = glm::normalize(p2 - p1);
			m_cells[i].applyForce(dt,  force * dir);
			m_cells[j].applyForce(dt, -force * dir);
		}
	}
}

void CellularLife::ImGui(entt::registry& R) {
	ImGui::Begin("Cellular Life");
	if (ImGui::Button("Reset Positions")) {
		/*std::vector<Point2DParameter>& pts = getPointsList(R);
		int i = 0;
		for (Point2DParameter& pt : pts) {
			entt::entity e = pt.getEntity();
			m_cells[i] = Cell(e);
			i++;
		}*/
		resetPositions(R);
	}
	ImGui::SliderFloat("Damping Coef", &m_dampingCoef, 0.0f, 13.0f);
	ImGui::SliderFloat("Attraction Force", &m_attraction, 0.0f, 10.0f); 
	ImGui::SliderFloat("Repulsion margin", &m_repulsionMargin, 0.4f, 1.0f);
	ImGui::SliderFloat("Container Radius", &m_maxRadius, 0.1f, 0.75f);
	ImGui::End();
}

std::vector<Point2DParameter>& CellularLife::getPointsList(entt::registry& R) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(m_layer);
	return ((ListOfPoints2DParameter*)params.list[0].get())->getList();
}

void CellularLife::checkEntityValidity(entt::registry& R) {
	if (!R.valid(m_cells[0].m_entity)) {
		std::vector<Point2DParameter>& pts = getPointsList(R);
		int i = 0;
		for (Point2DParameter& pt : pts) {
			entt::entity e = pt.getEntity();
			if (i < m_cells.size())
				m_cells[i].m_entity = e;
			else
				m_cells.emplace_back(e);
			i++;
		}
	}
	std::vector<Point2DParameter>& pts = getPointsList(R);
	if (pts.size() < m_cells.size()) {
		m_cells.resize(pts.size());
	}
}