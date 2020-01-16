#include "InputState_Translate.hpp"

#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "Debugging/Log.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Instance.hpp"

InputState_Translate::InputState_Translate(InputSystem* inputSystem, entt::entity targetID)
	: IInputState(inputSystem),
	  m_targetID(targetID),
	  m_mouseInitialPosInScreen(InputSystem::MousePositionInScreen()),
	  m_initialMat(I.registry().get<Cmp::TransformMatrix>(targetID).val)
{}

void InputState_Translate::onLeftClicUp() {
	m_inputSystem->setState<InputState_Rest>();
}

void InputState_Translate::update() {
	glm::vec2 dl = InputSystem::MousePositionInScreen() - m_mouseInitialPosInScreen;
	dl = glm::inverse(I.getParentModelMatrix(m_targetID)) * glm::vec3(dl, 0.0f);
	I.registry().get<Cmp::TransformMatrix>(m_targetID).val = glm::translate(glm::mat3(1.0f), dl) * m_initialMat;
}