#pragma once

#include "_PopupWindow.hpp"

class PopupWindow_AreYouSure : public PopupWindow {
public:
	PopupWindow_AreYouSure(PopupWindow* windowWaitingForConfirmation);
	~PopupWindow_AreYouSure() = default;

	void Open(const std::string& warnMessage);
private:
	void Show() override;
	void OnConfirmation() override;

private:
	std::string m_warnMessage;
	PopupWindow* m_windowWaitingForConfirmation;
};