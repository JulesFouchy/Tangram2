#include "InputState_DBTranslation.hpp"

#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "Debugging/Log.hpp"

#include "App.hpp" // TODO remove me

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

InputState_DBTranslation::InputState_DBTranslation(InputSystem* inputSystem)
	: IInputState(inputSystem), m_mouseInitialPosInScreen(InputSystem::MousePositionInScreen()),
	  m_DBInitialMat(m_inputSystem->m_registry.get<Cmp::TransformMatrix>(App::Get().m_drawingBoardId).val)
{}

void InputState_DBTranslation::onLeftClicUp() {
	m_inputSystem->setState<InputState_Rest>();
}

void InputState_DBTranslation::update() {
	entt::entity id = App::Get().m_drawingBoardId;
	glm::vec2 dl = InputSystem::MousePositionInScreen() - m_mouseInitialPosInScreen;
	m_inputSystem->m_registry.get<Cmp::TransformMatrix>(id).val = glm::translate(glm::mat3(1.0f), dl) * m_DBInitialMat;
}