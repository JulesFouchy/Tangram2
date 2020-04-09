#pragma once

#include "IInputState.hpp"

#include <functional>
#include <memory>

class Instance;
class PopupWindow;

class InputState_GUI : public IInputState {
public:
	InputState_GUI(InputSystem& inputSystem, std::function<std::unique_ptr<PopupWindow>()> windowConstructor);
	~InputState_GUI() = default;

	void update(entt::registry& R) override;

private:
	std::unique_ptr<PopupWindow> m_window;
};