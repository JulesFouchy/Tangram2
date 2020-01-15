#include "InputState_Rest.hpp"

#include "InputSystem.hpp"

#include "InputState_DBTranslation.hpp"

#include "Debugging/Log.hpp"

InputState_Rest::InputState_Rest(InputSystem* inputSystem)
	: IInputState(inputSystem)
{}

void InputState_Rest::onLeftClicDown() {
	if (InputSystem::KeyIsDown(SDL_SCANCODE_SPACE))
		m_inputSystem->setState<InputState_DBTranslation>();
}