#pragma once

#include "IInputState.hpp"

#include "MouseButtons.hpp"

class InputState_Translate : public IInputState {
public:
	InputState_Translate(Instance& instance, entt::entity targetID, MouseButton enterExitButton);
	~InputState_Translate() = default;

	void onLeftClicUp() override;
	void onWheelUp() override;

	void update() override;

private:
	entt::entity m_targetID;
	glm::vec2 m_mouseInitialPosInScreen;
	glm::mat3 m_initialMat;
	MouseButton m_enterExitButton;
	Uint32 timeInputStart;
};