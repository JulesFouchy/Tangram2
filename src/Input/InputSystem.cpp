#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

InputSystem::InputSystem(entt::registry& registry)
	: ISystem(registry)
{
	setState<InputState_Rest>();
}