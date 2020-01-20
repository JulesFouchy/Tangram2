#pragma once

#include <SDL2/SDL_scancode.h>

#include "Debugging/Log.hpp"

class InputSystem;
class Instance;

class IInputState {
public:
	IInputState(InputSystem* inputSystem);
	virtual ~IInputState() = default;

	virtual void update() {}

	virtual void onLeftClicDown() {}
	virtual void onLeftClicUp() {}
	virtual void onRightClicDown() {}
	virtual void onRightClicUp() {}
	virtual void onKeyDown(SDL_Scancode key) {}
	virtual void onKeyUp(SDL_Scancode key) {}
	virtual void onWheelScroll(float dl) {}

protected:
	InputSystem* m_inputSystem;
	Instance& I;
};