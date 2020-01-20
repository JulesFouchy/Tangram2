#include "Window_SaveProject.hpp"

#include "Instance.hpp"

#include "Helper/File.hpp"

#include <imgui/imgui.h>
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "Debugging/Log.hpp"


Window_SaveAsProject::Window_SaveAsProject(Instance& instance)
	: PopupWindow_WithConfirmationWarning("Saving project as"), I(instance),
	  m_folderpathPicker(FileFilter::None), m_projectName(instance.inputSystem().I.m_projectName)
{
	m_folderpathPicker.setFilepath(I.m_projectLocation);
}

void Window_SaveAsProject::Show() {
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

void Window_SaveAsProject::OnConfirmation() {
	std::string projectPath = projectFullPath();
	if (!MyFile::Exists(projectPath))
		std::filesystem::create_directory(projectPath);
	I.saveProject(projectPath);
}

bool Window_SaveAsProject::WarnIf() {
	return MyFile::Exists(projectFullPath()) && projectFullPath().compare(I.getProjectPath());
}

std::string Window_SaveAsProject::WarningMessage() {
	return "This project already exists and will be overwritten :\n" + projectFullPath();
}

std::string Window_SaveAsProject::projectFullPath() {
	return m_folderpathPicker.getFilepath() + "/" + (m_projectName.empty() ? "UntitledProject" : m_projectName);
}