#pragma once

#include "IInputState.hpp"

#include <glm/glm.hpp>

class InputState_DBTranslation : public IInputState {
public:
	InputState_DBTranslation(InputSystem* inputSystem);
	~InputState_DBTranslation() = default;

	void onLeftClicUp() override;

	void update() override;

private:
	glm::vec2 m_mouseInitialPosInScreen;
	glm::vec2 m_DBInitialPosInScreen;
};