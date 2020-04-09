#include "InputState_Translate.hpp"
#include "InputState_Rest.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Systems/HistoryManager.hpp"

#include "Core/GetMatrix.hpp"

#include "Settings/Settings.hpp"

#include "Debugging/Log.hpp"

#include <SDL2/SDL.h>


InputState_Translate::InputState_Translate(InputSystem& inputSystem, entt::entity targetID, MouseButton enterExitButton)
	: IInputState(inputSystem),
	  m_targetID(targetID),
	  m_mouseInitialPosInScreen(DisplayInfos::MousePositionInScreen()),
	  m_initialMat(I.registry().get<Cmp::TransformMatrix>(targetID).val()),
	  m_enterExitButton(enterExitButton),
	  timeInputStart(SDL_GetTicks())
{}

void InputState_Translate::onLeftClicUp(entt::registry& R) {
	if (m_enterExitButton == MouseButton::Left) {
		if (SDL_GetTicks() - timeInputStart > Settings::GetSELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS()) {
			// Save transform in history
			HistoryManager::RecordTransform(R, m_targetID, m_initialMat);
		}
		else {
			// Undo modifications
			R.replace<Cmp::TransformMatrix>(m_targetID, m_initialMat);
		}
		// Change State
		m_inputSystem.setState<InputState_Rest>();
	}
}

void InputState_Translate::onWheelUp(entt::registry& R) {
	if (m_enterExitButton == MouseButton::Wheel) {
		HistoryManager::RecordTransform(R, m_targetID, m_initialMat);
		m_inputSystem.setState<InputState_Rest>();
	}
}

void InputState_Translate::update(entt::registry& R) {
	glm::vec2 dl = DisplayInfos::MousePositionInScreen() - m_mouseInitialPosInScreen;
	dl = glm::inverse(TNG::GetParentModelMatrix(R, m_targetID)) * glm::vec3(dl, 0.0f);
	R.replace<Cmp::TransformMatrix>(m_targetID, glm::translate(glm::mat3(1.0f), dl) * m_initialMat);
}