#pragma once

#include <SDL2/SDL_scancode.h>
#include <entt/entt.hpp>

#include "Debugging/Log.hpp"

class InputSystem;

class IInputState {
public:
	IInputState(InputSystem& inputSystem) 
		: m_inputSystem(inputSystem)
	{}
	virtual ~IInputState() = default;

	virtual void update(entt::registry& R) {}

	virtual void onLeftClicDown(entt::registry& R, entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) {}
	virtual void onLeftClicUp(entt::registry& R) {}
	virtual void onRightClicDown(entt::registry& R) {}
	virtual void onRightClicUp(entt::registry& R) {}
	virtual void onWheelDown(entt::registry& R) {}
	virtual void onWheelUp(entt::registry& R) {}
	virtual void onKeyDown(entt::registry& R, SDL_Scancode key) {}
	virtual void onKeyUp(entt::registry& R, SDL_Scancode key) {}
	virtual void onWheelScroll(entt::registry& R, float dl) {}

protected:
	InputSystem& m_inputSystem;
};