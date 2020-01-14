#include "InputState_DBTranslation.hpp"

#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "Debugging/Log.hpp"

InputState_DBTranslation::InputState_DBTranslation(InputSystem* inputSystem)
	: IInputState(inputSystem)
{
	spdlog::warn("DB");
}

void InputState_DBTranslation::onKeyUp(SDL_Scancode key) {
	if (key == SDL_SCANCODE_SPACE)
		m_inputSystem->setState<InputState_Rest>();
}