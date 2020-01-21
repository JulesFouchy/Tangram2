#include "Window_SaveAsProject.hpp"

#include "Instance.hpp"

#include "Helper/File.hpp"

#include <imgui/imgui.h>
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "Debugging/Log.hpp"

#include "App.hpp"


Window_SaveAsProject::Window_SaveAsProject(Instance& instance)
	: PopupWindow_WithConfirmationWarning("Saving project as"), I(instance),
	  m_folderpathPicker(FileFilter::None), m_projectName(instance.inputSystem().I.m_projectName)
{
	m_folderpathPicker.setFilepath(I.m_projectLocation);
}

void Window_SaveAsProject::Show() {
	BeginWindow();
	ImGui::InputTextWithHint("Name", "UntitledProject", &m_projectName);
	m_folderpathPicker.ShowFolderBrowser();
	if (MyFile::Exists(m_folderpathPicker.getFilepath())) {
		auto it = App::Get().itToInstanceWithPath(projectFullPath());
		if (it != App::Get().m_instances.end() && &*it != &I)
			ImGui::TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), "A project with the same path is already open !\nClose the other instance first if you want to overwrite it.");
		else {
			ConfirmationButton();
			ImGui::Separator();
		}
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
	I.m_bUserChoseProjectName = true;
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