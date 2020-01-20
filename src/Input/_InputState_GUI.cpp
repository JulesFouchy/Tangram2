#include "_InputState_GUI.hpp"

#include "InputSystem.hpp"
#include "InputState_Rest.hpp"

#include <imgui/imgui.h>

_InputState_GUI::_InputState_GUI(InputSystem* inputSystem)
	: IInputState(inputSystem)
{}

void _InputState_GUI::update() {
	if (!m_window->IsOpen()) // user closed window. Don't apply "onConfirmation()"
		m_inputSystem->setState<InputState_Rest>();
	else
		m_window->Show();
}