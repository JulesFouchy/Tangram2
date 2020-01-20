#include "InputState_GUI.hpp"

#include "InputState_Rest.hpp"

#include "GUI/PopupWindow/_PopupWindow.hpp"
#include "Instance.hpp"

InputState_GUI::InputState_GUI(Instance& instance, std::function<std::unique_ptr<PopupWindow>(Instance&)> windowConstructor)
	: IInputState(instance)
{
	m_window = windowConstructor(I);
	m_window->Open();
}

void InputState_GUI::update() {
	if (!m_window->IsOpen())
		I.inputSystem().setState<InputState_Rest>();
	else
		m_window->Show_IfOpen();
}