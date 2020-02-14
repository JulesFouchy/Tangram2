#pragma once

#include "IInputState.hpp"

class InputState_Rest : public IInputState {
public:
	InputState_Rest(Instance& instance);
	~InputState_Rest() = default;

	void onLeftClicDown() override;
	void onWheelDown() override;
	void onWheelScroll(float dl) override;
	void onKeyDown(SDL_Scancode key) override;
};