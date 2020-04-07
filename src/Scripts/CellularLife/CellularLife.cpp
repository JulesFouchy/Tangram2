#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

#include "Components/ParametersList.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/Random.hpp"

#include <imgui/imgui.h>

CellularLife::CellularLife(entt::registry& R, LayersManager& layersM) {
	m_layer = layersM.createFragmentLayer("res/shaders/testCellDistortion.frag");
	resetPositions(R);
}

void CellularLife::resetPositions(entt::registry& R) {
	float ratio = DisplayInfos::Ratio();
	std::vector<Point2DParameter>& pts = getPointsList(R);
	for (Point2DParameter& pt : pts) {
		entt::entity e = pt.getEntity();
		TNG::SetPosition(R, e, glm::vec2((m_rand.Float()-0.5f)*ratio, m_rand.Float()-0.5f));
	}
}

void CellularLife::loopIteration(float dt, entt::registry& R) {
	std::vector<Point2DParameter>& pts = getPointsList(R);
	for (Point2DParameter& pt : pts) {
		entt::entity e = pt.getEntity();
		glm::vec2 pos = TNG::GetPosition(R, e);
		//pos.x += 2.0f * dt;
		TNG::SetPosition(R, e, pos);
	}
}

void CellularLife::ImGui(entt::registry& R) {
	ImGui::Begin("Cellular Life");
	if (ImGui::Button("Reset Positions")) {
		resetPositions(R);
	}
	ImGui::End();
}

std::vector<Point2DParameter>& CellularLife::getPointsList(entt::registry& R) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(m_layer);
	return ((ListOfPoints2DParameter*)params.list[0].get())->getList();
}