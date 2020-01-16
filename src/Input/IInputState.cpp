#include "IInputState.hpp"

#include "InputSystem.hpp"

IInputState::IInputState(InputSystem* inputSystem)
	: m_inputSystem(inputSystem), I(inputSystem->I)
{}