#include "InputState_Rest.hpp"

#include "InputSystem.hpp"

#include "InputState_DBTranslation.hpp"

#include "Debugging/Log.hpp"

InputState_Rest::InputState_Rest(InputSystem* inputSystem)
	: IInputState(inputSystem)
{
	spdlog::warn("REST");
}

void InputState_Rest::onKeyDown(SDL_Scancode key) {
	if (key == SDL_SCANCODE_SPACE)
		m_inputSystem->setState<InputState_DBTranslation>();
}