#pragma once

#include "IInputState.hpp"

#include <glm/glm.hpp>

#include <entt/entt.hpp>

class InputState_Translate : public IInputState {
public:
	InputState_Translate(InputSystem* inputSystem, entt::entity targetID);
	~InputState_Translate() = default;

	void onLeftClicUp() override;

	void update() override;

private:
	entt::entity m_targetID;
	glm::vec2 m_mouseInitialPosInScreen;
	glm::mat3 m_initialMat;
};