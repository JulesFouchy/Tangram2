#include "_PopupWindow.hpp"

#include "Debugging/Log.hpp"

#include <imgui/imgui.h>

void PopupWindow::Open() {
	m_bMustShow = true;
}

void PopupWindow::Show_IfOpen() {
	if (m_bMustShow) {
		Show();
	}
}

PopupWindow::PopupWindow(const std::string& windowName)
	: m_bMustShow(false), m_name(windowName)
{
}

void PopupWindow::BeginWindow() {
	ImGui::Begin(m_name.c_str(), &m_bMustShow);
}

void PopupWindow::ConfirmationButton() {
	if (ImGui::Button("OK !")) {
		Confirm();
	}
}

void PopupWindow::EndWindow() {
	ImGui::End();
}