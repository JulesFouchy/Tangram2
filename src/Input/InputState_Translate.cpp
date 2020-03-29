#include "InputState_Translate.hpp"
#include "InputState_Rest.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Systems/HistoryManager.hpp"

#include "Settings/Settings.hpp"

#include "Debugging/Log.hpp"

#include <SDL2/SDL.h>


InputState_Translate::InputState_Translate(Instance& instance, entt::entity targetID, MouseButton enterExitButton)
	: IInputState(instance),
	  m_targetID(targetID),
	  m_mouseInitialPosInScreen(DisplayInfos::MousePositionInScreen()),
	  m_initialMat(I.registry().get<Cmp::TransformMatrix>(targetID).val()),
	  m_enterExitButton(enterExitButton),
	  timeInputStart(SDL_GetTicks())
{}

void InputState_Translate::onLeftClicUp() {
	if (m_enterExitButton == MouseButton::Left) {
		if (SDL_GetTicks() - timeInputStart > Settings::GetSELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS()) {
			// Save transform in history
			HistoryManager::RecordTransform(I.registry(), m_targetID, m_initialMat);
		}
		else {
			// Undo modifications
			I.registry().replace<Cmp::TransformMatrix>(m_targetID, m_initialMat);
		}
		// Activ history is now Transform
		I.registry().reset<entt::tag<"ActiveHistoryIsParameter"_hs>>(m_targetID);
		// Change State
		I.inputSystem().setState<InputState_Rest>();
	}
}

void InputState_Translate::onWheelUp() {
	if (m_enterExitButton == MouseButton::Wheel) {
		HistoryManager::RecordTransform(I.registry(), m_targetID, m_initialMat);
		I.inputSystem().setState<InputState_Rest>();
	}
}

void InputState_Translate::update() {
	glm::vec2 dl = DisplayInfos::MousePositionInScreen() - m_mouseInitialPosInScreen;
	dl = glm::inverse(I.getParentModelMatrix(m_targetID)) * glm::vec3(dl, 0.0f);
	I.registry().replace<Cmp::TransformMatrix>(m_targetID, glm::translate(glm::mat3(1.0f), dl) * m_initialMat);
}