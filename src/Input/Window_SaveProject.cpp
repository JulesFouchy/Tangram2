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
	  m_folderpathPicker(FileFilter::None), m_projectName(inputSystem->I.m_projectName)
{
	m_folderpathPicker.setFilepath(inputSystem->I.m_projectLocation);
}

void Window_SaveProject::Show() {
	BeginWindow();
	ImGui::InputText("Name", &m_projectName);
	m_folderpathPicker.ShowFolderBrowser();
	if (MyFile::Exists(m_folderpathPicker.getFilepath())) {
		ConfirmationButton();
		ImGui::Separator();
	}
	else {
		ImGui::TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), "Location doesn't exist !");
	}
	EndWindow();
}

void Window_SaveProject::OnConfirmation() {
	std::string projectPath = projectFullPath();
	if (!MyFile::Exists(projectPath))
		std::filesystem::create_directory(projectPath);
	m_inputSystem->I.saveProject(projectPath);
	m_inputSystem->setState<InputState_Rest>();
}

bool Window_SaveProject::WarnIf() {
	return MyFile::Exists(projectFullPath());
}
std::string Window_SaveProject::WarningMessage() {
	return "This project already exists and will be overwritten :\n" + projectFullPath();
}

std::string Window_SaveProject::projectFullPath() {
	return m_folderpathPicker.getFilepath() + "/" + (m_projectName.empty() ? "UntitledProject" : m_projectName);
}