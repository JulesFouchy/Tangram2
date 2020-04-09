#include "InputState_GUI.hpp"

#include "InputState_Rest.hpp"

#include "InputSystem.hpp"

#include "GUI/PopupWindow/_PopupWindow.hpp"

InputState_GUI::InputState_GUI(InputSystem& inputSystem, std::function<std::unique_ptr<PopupWindow>()> windowConstructor)
	: IInputState(inputSystem)
{
	m_window = windowConstructor();
	m_window->Open();
}

void InputState_GUI::update(entt::registry& R) {
	if (!m_window->IsOpen())
		m_inputSystem.setState<InputState_Rest>();
	else
		m_window->Show_IfOpen();
}