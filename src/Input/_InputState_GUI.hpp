#pragma once

#include "IInputState.hpp"
#include "InputSystem.hpp"
#include "InputState_Rest.hpp"
#include "GUI/PopupWindow/_PopupWindow.hpp"
#include "Window_SaveProject.hpp"

#include "Debugging/Log.hpp"

#include <memory>

typedef Window_SaveProject WindowType;
//template <typename T>
class _InputState_GUI : public IInputState {
public:
	_InputState_GUI::_InputState_GUI(InputSystem* inputSystem)
		: IInputState(inputSystem), m_window(inputSystem)
	{
		spdlog::info("GUI in");
		m_window.Open();
	}
	~_InputState_GUI() {
		spdlog::info("GUI out");
	}

	void _InputState_GUI::update() override {
		if (!m_window.IsOpen()) // user closed window. Don't apply "onConfirmation()"
		{
			m_inputSystem->setState<InputState_Rest>();
		}
		else
			m_window.Show_IfOpen();
	}

protected:
	WindowType m_window;
};