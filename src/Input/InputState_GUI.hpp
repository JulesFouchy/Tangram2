#pragma once

#include "IInputState.hpp"

class Instance;
class PopupWindow;

class InputState_GUI : public IInputState {
public:
	InputState_GUI::InputState_GUI(Instance& instance, std::function<std::unique_ptr<PopupWindow>(Instance&)> windowConstructor);
	~InputState_GUI() = default;

	void InputState_GUI::update() override;

private:
	std::unique_ptr<PopupWindow> m_window;
};