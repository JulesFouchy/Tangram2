#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

#include "Components/ParametersList.hpp"

#include "Parameters/ConcreteParameters.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

CellularLife::CellularLife(LayersManager& layersM) {
	m_layer = layersM.createFragmentLayer("res/shaders/testCellDistortion.frag");
}

void CellularLife::loopIteration(float dt, entt::registry& R) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(m_layer);
	std::vector<Point2DParameter>& pts = ((ListOfPoints2DParameter*) params.list[0].get())->getList();
	for (Point2DParameter& pt : pts) {
		entt::entity e = pt.getEntity();
		glm::vec2 pos = TNG::GetPosition(R, e);
		pos.x += 2.0f * dt;
		TNG::SetPosition(R, e, pos);
	}
}