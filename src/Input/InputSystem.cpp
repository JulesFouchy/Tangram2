#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

InputSystem::InputSystem()
{
	setState<InputState_Rest>();
}