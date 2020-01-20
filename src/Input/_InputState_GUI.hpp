#pragma once

#include "IInputState.hpp"
#include "GUI/PopupWindow/_PopupWindow.hpp"

#include <memory>

class _InputState_GUI : public IInputState {
public:
	_InputState_GUI(InputSystem* inputSystem);
	~_InputState_GUI() = default;

	void update() override;

protected:
	std::unique_ptr<PopupWindow> m_window;
};