#include "InputState_Rest.hpp"

#include "InputSystem.hpp"

#include "InputState_Translate.hpp"

#include "Components/TransformMatrix.hpp"

#include "Instance.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

InputState_Rest::InputState_Rest(InputSystem* inputSystem)
	: IInputState(inputSystem)
{}

void InputState_Rest::onLeftClicDown() {
	if (InputSystem::KeyIsDown(SDL_SCANCODE_SPACE))
		m_inputSystem->m_currentState = std::make_unique<InputState_Translate>(m_inputSystem, I.drawingBoardId());
	else {
		entt::entity hoveredLayer = I.layersManager().layerHoveredByMouse();
		if (I.registry().valid(hoveredLayer))
			m_inputSystem->m_currentState = std::make_unique<InputState_Translate>(m_inputSystem, hoveredLayer);
	}
}

void InputState_Rest::onWheelScroll(float dl) {
	glm::mat3& mat = I.registry().get<Cmp::TransformMatrix>(I.drawingBoardId()).val;
	mat = glm::scale(mat, glm::vec2(pow(0.95f, -dl)));
}