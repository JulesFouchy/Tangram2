#pragma once

#include "IInputState.hpp"

#include <memory>

class InputSystem {
friend class IInputState;
friend class InputState_Rest;
friend class InputState_Translate;
friend class InputState_GUI;
friend class Window_SaveAsProject;
public:
	InputSystem();
	~InputSystem() = default;

	inline void update() { m_currentState->update(); }

	inline void onLeftClicDown(entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) { m_currentState->onLeftClicDown(clickedEntity, clickedLayer, rSelectedLayer); }
	inline void onLeftClicUp()              { m_currentState->onLeftClicUp(); }
	inline void onRightClicDown()           { m_currentState->onRightClicDown(); }
	inline void onRightClicUp()             { m_currentState->onRightClicUp(); }
	inline void onWheelDown()               { m_currentState->onWheelDown(); }
	inline void onWheelUp()                 { m_currentState->onWheelUp(); }
	inline void onKeyDown(SDL_Scancode key) { m_currentState->onKeyDown(key); }
	inline void onKeyUp(SDL_Scancode key)	{ m_currentState->onKeyUp(key); }
	inline void onWheelScroll(float dl)     { m_currentState->onWheelScroll(dl); }

private:
	template <typename State>
	inline void setState() { m_currentState = std::make_unique<State>(*this); }
	template <typename WindowType>
	inline void setGUIState() { m_currentState = std::make_unique<InputState_GUI>(*this, [](Instance& instance) { return std::make_unique<WindowType>(instance); });}

private:
	std::unique_ptr<IInputState> m_currentState;
};