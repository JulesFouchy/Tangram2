#include "_AreYouSure.hpp"

#include "imgui/imgui.h"

PopupWindow_AreYouSure::PopupWindow_AreYouSure(PopupWindow* windowWaitingForConfirmation)
	: PopupWindow("Are you sure ?"), m_windowWaitingForConfirmation(windowWaitingForConfirmation)
{
}

void PopupWindow_AreYouSure::Open(const std::string& warnMessage) {
	PopupWindow::Open();
	m_warnMessage = warnMessage;
}

void PopupWindow_AreYouSure::Show() {
	BeginWindow();
	//
	ImGui::TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), m_warnMessage.c_str());
	//
	ImGui::Separator();
	ConfirmationButton();
	EndWindow();
}

void PopupWindow_AreYouSure::OnConfirmation() {
	m_windowWaitingForConfirmation->Confirm(true);
}