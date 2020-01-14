#pragma once

#include "IInputState.hpp"

class InputState_Rest : public IInputState {
public:
	InputState_Rest(InputSystem* inputSystem);
	~InputState_Rest() = default;

	void onKeyDown(SDL_Scancode key) override;
};