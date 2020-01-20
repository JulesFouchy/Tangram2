#pragma once

#include "IInputState.hpp"
#include "InputSystem.hpp"
#include "InputState_Rest.hpp"
#include "GUI/PopupWindow/_PopupWindow.hpp"
#include "Window_SaveProject.hpp"

#include "Debugging/Log.hpp"

#include "GUI/PopupWindow/_PopupWindow.hpp"

#include <functional>

#include <memory>

class _InputState_GUI : public IInputState {
public:
	_InputState_GUI::_InputState_GUI(InputSystem* inputSystem, std::function<std::unique_ptr<PopupWindow>(InputSystem*)> windowConstructor)
		: IInputState(inputSystem)
	{
		m_window = windowConstructor(inputSystem);
		m_window->Open();
	}
	~_InputState_GUI() = default;

	void _InputState_GUI::update() override {
		if (!m_window->IsOpen())
			m_inputSystem->setState<InputState_Rest>();
		else
			m_window->Show_IfOpen();
	}

protected:
	std::unique_ptr<PopupWindow> m_window;
};