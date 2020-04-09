#pragma once

#include "IInputState.hpp"

#include "MouseButtons.hpp"

#include <glm/glm.hpp>

#include <entt/entt.hpp>

class InputState_Translate : public IInputState {
public:
	InputState_Translate(InputSystem& inputSystem, entt::entity targetID, MouseButton enterExitButton);
	~InputState_Translate() = default;

	void onLeftClicUp(entt::registry& R) override;
	void onWheelUp(entt::registry& R) override;

	void update(entt::registry& R) override;

private:
	entt::entity m_targetID;
	glm::vec2 m_mouseInitialPosInScreen;
	glm::mat3 m_initialMat;
	MouseButton m_enterExitButton;
	Uint32 timeInputStart;
};