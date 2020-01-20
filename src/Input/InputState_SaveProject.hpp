#pragma once

#include "_InputState_GUI.hpp"
#include "Window_SaveProject.hpp"

#include "Debugging/Log.hpp"

class InputState_SaveProject : public _InputState_GUI {
public:
	InputState_SaveProject(InputSystem* inputSystem)
		: _InputState_GUI(inputSystem)
	{
		spdlog::info("save project");
		m_window = std::make_unique<Window_SaveProject>(inputSystem);
		m_window->Open();
	}
	~InputState_SaveProject() = default;
};