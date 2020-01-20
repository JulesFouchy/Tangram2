#pragma once

#include "IInputState.hpp"
#include "InputSystem.hpp"
#include "InputState_Rest.hpp"
#include "GUI/PopupWindow/_PopupWindow.hpp"
#include "Window_SaveProject.hpp"

#include "Debugging/Log.hpp"

#include "GUI/PopupWindow/_PopupWindow.hpp"

#include <functional>

#include "Instance.hpp"

#include <memory>

class _InputState_GUI : public IInputState {
public:
	_InputState_GUI::_InputState_GUI(Instance& instance, std::function<std::unique_ptr<PopupWindow>(Instance&)> windowConstructor)
		: IInputState(instance)
	{
		m_window = windowConstructor(I);
		m_window->Open();
	}
	~_InputState_GUI() = default;

	void _InputState_GUI::update() override {
		if (!m_window->IsOpen())
			I.inputSystem().setState<InputState_Rest>();
		else
			m_window->Show_IfOpen();
	}

protected:
	std::unique_ptr<PopupWindow> m_window;
};