#include "InputState_DBTranslation.hpp"

#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "Debugging/Log.hpp"

#include "App.hpp" // TODO remove me

#include "Components/Translation.hpp"

InputState_DBTranslation::InputState_DBTranslation(InputSystem* inputSystem)
	: IInputState(inputSystem), m_mouseInitialPosInScreen(InputSystem::MousePositionInScreen()),
	m_DBInitialPosInScreen(m_inputSystem->m_registry.get<Cmp::Translation>(App::Get().m_drawingBoardId).val)
{}

void InputState_DBTranslation::onLeftClicUp() {
	m_inputSystem->setState<InputState_Rest>();
}

void InputState_DBTranslation::update() {
	entt::entity id = App::Get().m_drawingBoardId;
	glm::vec2 dl = InputSystem::MousePositionInScreen() - m_mouseInitialPosInScreen + m_DBInitialPosInScreen;
	m_inputSystem->m_registry.get<Cmp::Translation>(id).val = dl ;
}