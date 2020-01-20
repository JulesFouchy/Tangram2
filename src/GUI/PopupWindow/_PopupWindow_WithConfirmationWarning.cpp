#include "_PopupWindow_WithConfirmationWarning.hpp"

#include "Debugging/Log.hpp"
#include <imgui/imgui.h>

PopupWindow_WithConfirmationWarning::PopupWindow_WithConfirmationWarning(const std::string& windowName)
	: PopupWindow(windowName), m_warningWindow(this)
{
}

void PopupWindow_WithConfirmationWarning::Show_IfOpen() {
	PopupWindow::Show_IfOpen();
	m_warningWindow.Show_IfOpen();
}

void PopupWindow_WithConfirmationWarning::Confirm(bool iReadTheWarningAndIAmStillSure) {
	if (WarnIf() && !iReadTheWarningAndIAmStillSure) {
		m_warningWindow.Open(WarningMessage());
	}
	else {
		OnConfirmation();
		m_bMustShow = false;
	}
}