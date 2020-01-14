#include "InputSystem.hpp"

#include "InputState_Rest.hpp"

#include "Helper/DisplayInfos.hpp"

#include "SDL2/SDL.h"

float InputSystem::HorizontalDPI;
float InputSystem::VerticalDPI;

InputSystem::InputSystem(entt::registry& registry)
	: ISystem(registry)
{
	setState<InputState_Rest>();
}

void InputSystem::Initialize() {
	SDL_GetDisplayDPI(0, nullptr, &HorizontalDPI, &VerticalDPI);
}

glm::vec2 InputSystem::MousePositionInPixels(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(x, DisplayInfos::Height() - y);
}

glm::vec2 InputSystem::MousePositionInInches() {
	return MousePositionInPixels() / glm::vec2(HorizontalDPI, VerticalDPI);
}

bool InputSystem::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}