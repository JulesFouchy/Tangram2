#pragma once

#include <string>

class PopupWindow {
friend class PopupWindow_AreYouSure;
public:
	~PopupWindow() = default;

	// Functions to override
	virtual void Open();
	virtual void Show() = 0;
protected:
	virtual void OnConfirmation() = 0;

	// Base functions
public:
	virtual void Show_IfOpen();
	inline bool IsOpen() { return m_bMustShow; }
protected:
	PopupWindow(const std::string& windowName);

	void BeginWindow();
	void ConfirmationButton();
	void EndWindow();

	virtual void Confirm(bool iAmSure = false) { OnConfirmation(); m_bMustShow = false; }; // indirection required by PopupWindow_WithConfirmationWarning to add checks

protected:
	bool m_bMustShow;
private:
	std::string m_name;
};