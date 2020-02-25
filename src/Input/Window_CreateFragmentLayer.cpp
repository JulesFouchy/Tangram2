#include "Window_CreateFragmentLayer.hpp"

#include "Instance.hpp"

#include <imgui/imgui.h>
#include "Helper/File.hpp"


Window_CreateFragmentLayer::Window_CreateFragmentLayer(Instance& instance)
	: PopupWindow("Creating a Fragment layer"), I(instance),
	  m_filepathPicker(FileFilter::FragmentShader)
{
	m_filepathPicker.setFilepath("");
}
void Window_CreateFragmentLayer::Show() {
	BeginWindow();
	//
	ImGui::Text("Open : "); ImGui::SameLine();
	m_filepathPicker.ShowOpenfilename();
	ImGui::Separator();
	//
	ConfirmationButton(); ImGui::SameLine();
	//
	EndWindow();
}

void Window_CreateFragmentLayer::OnConfirmation() {
	I.layersManager().createFragmentLayer(m_filepathPicker.getFilepath());
}