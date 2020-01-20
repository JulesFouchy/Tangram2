#pragma once

#include <SDL2/SDL_scancode.h>

#include "Debugging/Log.hpp"

class Instance;

class IInputState {
public:
	IInputState(Instance& instance);
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
	Instance& I;
};