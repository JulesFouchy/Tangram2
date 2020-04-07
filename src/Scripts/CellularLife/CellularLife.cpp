#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

#include "Components/ParametersList.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/Random.hpp"

#include <imgui/imgui.h>

CellularLife::CellularLife(entt::registry& R, LayersManager& layersM)
	: m_dampingCoef(0.5f)
{
	m_layer = layersM.createFragmentLayer("res/shaders/testCellDistortion.frag");
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

void CellularLife::loopIteration(float dt, entt::registry& R) {
	for (Cell& cell : m_cells) {
		cell.applyDamping(dt, m_dampingCoef);
		cell.move(R, dt);
	}
}

void CellularLife::ImGui(entt::registry& R) {
	ImGui::Begin("Cellular Life");
	if (ImGui::Button("Reset Positions")) {
		std::vector<Point2DParameter>& pts = getPointsList(R);
		int i = 0;
		for (Point2DParameter& pt : pts) {
			entt::entity e = pt.getEntity();
			m_cells[i] = Cell(e);
			i++;
		}
		resetPositions(R);
	}
	ImGui::SliderFloat("Damping Coef", &m_dampingCoef, 0.0f, 1.0f);
	ImGui::End();
}

std::vector<Point2DParameter>& CellularLife::getPointsList(entt::registry& R) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(m_layer);
	return ((ListOfPoints2DParameter*)params.list[0].get())->getList();
}