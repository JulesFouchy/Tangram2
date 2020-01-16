#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "SDL2/SDL.h"

InputSystem::InputSystem(Instance& instance)
	: ISystem(instance)
{
	setState<InputState_Rest>();
}

bool InputSystem::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}