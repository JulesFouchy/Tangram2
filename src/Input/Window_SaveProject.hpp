#pragma once

#include "GUI/PopupWindow/_PopupWindow_WithConfirmationWarning.hpp"
#include "GUI/Widget/FilepathPicker.hpp"

class Instance;

class Window_SaveProject : public PopupWindow_WithConfirmationWarning {
public:
	Window_SaveProject(Instance& instance);
	~Window_SaveProject() = default;

	void Show() override;
protected:
	void OnConfirmation() override;
	bool WarnIf() override;
	std::string WarningMessage() override;

private:
	std::string projectFullPath();

private:
	Instance& I;
	GUIwidget_FilepathPicker m_folderpathPicker;
	std::string m_projectName;
};