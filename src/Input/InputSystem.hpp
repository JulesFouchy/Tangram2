#pragma once

#include "System/ISystem.hpp"

#include "IInputState.hpp"

#include <memory>

class InputSystem : public ISystem {
friend class IInputState;
friend class InputState_Rest;
friend class InputState_Translate;
public:
	InputSystem(Instance& instance);
	~InputSystem() = default;

	inline void update() { m_currentState->update(); }

	inline void onLeftClicDown()            { m_currentState->onLeftClicDown(); }
	inline void onLeftClicUp()              { m_currentState->onLeftClicUp(); }
	inline void onRightClicDown()           { m_currentState->onRightClicDown(); }
	inline void onRightClicUp()             { m_currentState->onRightClicUp(); }
	inline void onKeyDown(SDL_Scancode key) { m_currentState->onKeyDown(key); }
	inline void onKeyUp(SDL_Scancode key)	{ m_currentState->onKeyUp(key); }
	inline void onWheelScroll(float dl)     { m_currentState->onWheelScroll(dl); }

private:
	template <typename T>
	inline void setState() { m_currentState = std::make_unique<T>(this); }

private:
	std::unique_ptr<IInputState> m_currentState;
};