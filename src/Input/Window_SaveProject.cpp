#include "Window_SaveProject.hpp"

#include <imgui/imgui.h>
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "InputSystem.hpp"
#include "InputState_Rest.hpp"
#include "Instance.hpp"
#include "Helper/File.hpp"

#include "Debugging/Log.hpp"


Window_SaveProject::Window_SaveProject(InputSystem* inputSystem)
	: PopupWindow_WithConfirmationWarning("Saving project"), m_inputSystem(inputSystem),
	  m_filepathPicker(FileFilter::None)
{}

void Window_SaveProject::Show() {
	BeginWindow();
	ImGui::InputText("Name", &m_projectName);
	m_filepathPicker.ShowFolderBrowser();
	ImGui::Separator();
	ConfirmationButton();
	EndWindow();
}

void Window_SaveProject::OnConfirmation() {
	m_inputSystem->I.saveProject(m_filepathPicker.getFilepath() + "/" + m_projectName);
	m_inputSystem->setState<InputState_Rest>();
}

bool Window_SaveProject::WarnIf() {
	return false;//MyFile::Exists(m_filepathPicker.getFilepath());
}

std::string Window_SaveProject::WarningMessage() {
	return "This file already exists and will be overwritten : \n" + m_filepathPicker.getFilepath();
}