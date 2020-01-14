#pragma once

#include "System/ISystem.hpp"

#include "IInputState.hpp"

#include <memory>

#include <glm/glm.hpp>

class InputSystem : public ISystem {
friend class InputState_Rest;
friend class InputState_DBTranslation;
public:
	InputSystem(entt::registry& registry);
	~InputSystem() = default;
	static void Initialize();

	static glm::vec2 MousePositionInPixels();
	static glm::vec2 MousePositionInInches();
	static bool KeyIsDown(SDL_Scancode key);

	inline void update() override { m_currentState->update(); }

	inline void onLeftClicDown()            { m_currentState->onLeftClicDown(); }
	inline void onLeftClicUp()              { m_currentState->onLeftClicUp(); }
	inline void onRightClicDown()           { m_currentState->onRightClicDown(); }
	inline void onRightClicUp()             { m_currentState->onRightClicUp(); }
	inline void onKeyDown(SDL_Scancode key) { m_currentState->onKeyDown(key); }
	inline void onKeyUp(SDL_Scancode key)	{ m_currentState->onKeyUp(key); }

private:
	template <typename T> 
	inline void setState() { m_currentState = std::make_unique<T>(this); }

private:
	std::unique_ptr<IInputState> m_currentState;

	static float HorizontalDPI;
	static float VerticalDPI;
};