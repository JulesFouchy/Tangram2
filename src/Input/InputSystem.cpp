#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

InputSystem::InputSystem(Instance& instance)
	: ISystem(instance)
{
	setState<InputState_Rest>();
}