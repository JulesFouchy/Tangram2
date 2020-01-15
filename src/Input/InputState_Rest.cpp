#include "InputState_Rest.hpp"

#include "InputSystem.hpp"

#include "InputState_DBTranslation.hpp"

#include "Debugging/Log.hpp"

#include "Components/Scale.hpp"

#include "App.hpp"

InputState_Rest::InputState_Rest(InputSystem* inputSystem)
	: IInputState(inputSystem)
{}

void InputState_Rest::onLeftClicDown() {
	if (InputSystem::KeyIsDown(SDL_SCANCODE_SPACE))
		m_inputSystem->setState<InputState_DBTranslation>();
}

void InputState_Rest::onWheelScroll(float dl) {
	m_inputSystem->m_registry.get<Cmp::Scale>(App::Get().m_drawingBoardId).val *= pow(0.95f, -dl);
}