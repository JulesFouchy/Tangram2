#include "InputState_Translate.hpp"
#include "InputState_Rest.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Debugging/Log.hpp"


InputState_Translate::InputState_Translate(Instance& instance, entt::entity targetID)
	: IInputState(instance),
	  m_targetID(targetID),
	  m_mouseInitialPosInScreen(DisplayInfos::MousePositionInScreen()),
	  m_initialMat(I.registry().get<Cmp::TransformMatrix>(targetID).val())
{}

void InputState_Translate::onLeftClicUp() {
	I.inputSystem().setState<InputState_Rest>();
}

void InputState_Translate::update() {
	glm::vec2 dl = DisplayInfos::MousePositionInScreen() - m_mouseInitialPosInScreen;
	dl = glm::inverse(I.getParentModelMatrix(m_targetID)) * glm::vec3(dl, 0.0f);
	I.registry().replace<Cmp::TransformMatrix>(m_targetID, glm::translate(glm::mat3(1.0f), dl) * m_initialMat);
}