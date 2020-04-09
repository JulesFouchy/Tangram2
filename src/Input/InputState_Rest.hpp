#pragma once

#include "IInputState.hpp"

class InputState_Rest : public IInputState {
public:
	InputState_Rest(InputSystem& inputSystem);
	~InputState_Rest() = default;

	void onLeftClicDown(entt::registry& R, entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) override;
	void onWheelDown(entt::registry& R) override;
	void onWheelScroll(entt::registry& R, float dl) override;
	void onKeyDown(entt::registry& R, SDL_Scancode key) override;
};