#include "Window_ExportImage.hpp"

#include "Instance.hpp"

#include <imgui/imgui.h>
#include "Helper/File.hpp"
#include "Helper/DisplayInfos.hpp"

#include "Components/AspectRatio.hpp"

#include "Core/GetDrawingBoard.hpp"


Window_ExportImage::Window_ExportImage(Instance& instance)
	: PopupWindow_WithConfirmationWarning("Exporting image"), I(instance),
	  m_filepathPicker(FileFilter::Image)
{
	m_widthHeightRatioPicker.setRatio(I.registry().get<Cmp::AspectRatio>(TNG::GetDrawingBoard(I.registry())).val);
	m_filepathPicker.setFilepath("");
}
void Window_ExportImage::Show() {
	BeginWindow();
	//
	m_widthHeightRatioPicker.ShowWidthHeight();
	ImGui::Separator();
	//
	ImGui::Text("Save as : "); ImGui::SameLine();
	m_filepathPicker.ShowSavefilename();
	ImGui::Separator();
	//
	ConfirmationButton(); ImGui::SameLine();
	//
	EndWindow();
}

void Window_ExportImage::OnConfirmation() {
	I.renderSystem().exportImage(m_widthHeightRatioPicker.getWidth(),
		                         m_widthHeightRatioPicker.getHeight(), 
		                         m_filepathPicker.getFilepath()
	);
}

bool Window_ExportImage::WarnIf() {
	return MyFile::Exists(m_filepathPicker.getFilepath());
}

std::string Window_ExportImage::WarningMessage() {
	return "This file already exists and will be overwritten : \n" + m_filepathPicker.getFilepath();
}