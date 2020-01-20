#include "FilepathPicker.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "GUI/FileBrowser.hpp"

GUIwidget_FilepathPicker::GUIwidget_FilepathPicker(FileFilter filter)
	: m_filepath(""), m_fileFilter(filter)
{}

void GUIwidget_FilepathPicker::ShowOpenfilename() {
	ImGui::InputText("", &m_filepath);
	if (ImGui::Button("Choose file")) {
		std::string tmp = FileBrowser::GetFileOpen(m_fileFilter);
		if (tmp != "") {
			m_filepath = tmp;
		}
	}
}

void GUIwidget_FilepathPicker::ShowSavefilename() {
	ImGui::InputText("", &m_filepath);
	if (ImGui::Button("Choose destination")) {
		std::string tmp = FileBrowser::GetFileSave(m_fileFilter);
		if (tmp != "") {
			m_filepath = tmp;
		}
	}
}

void GUIwidget_FilepathPicker::ShowFolderBrowser() {
	ImGui::InputText("", &m_filepath); 
	ImGui::SameLine();
	if (ImGui::Button("Location")) {
		std::string tmp = FileBrowser::GetFolder();
		if (tmp != "") {
			m_filepath = tmp;
		}
	}
}