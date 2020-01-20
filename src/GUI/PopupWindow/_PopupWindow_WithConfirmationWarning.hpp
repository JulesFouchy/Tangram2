#pragma once

#include "_PopupWindow.hpp"
#include "_AreYouSure.hpp"

class PopupWindow_WithConfirmationWarning : public PopupWindow {
	// Functions to override
protected:
	virtual bool WarnIf() = 0;
	virtual std::string WarningMessage() = 0;

	// Base functions
public:
	void Show_IfOpen() override;
protected:
	PopupWindow_WithConfirmationWarning(const std::string& windowName);
	~PopupWindow_WithConfirmationWarning() = default;

	void Confirm(bool iReadTheWarningAndIAmStillSure = false) override;

private:
	PopupWindow_AreYouSure m_warningWindow;
};