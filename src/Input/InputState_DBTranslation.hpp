#pragma once

#include "IInputState.hpp"

class InputState_DBTranslation : public IInputState {
public:
	InputState_DBTranslation(InputSystem* inputSystem);
	~InputState_DBTranslation() = default;

	void onLeftClicUp() override;
};