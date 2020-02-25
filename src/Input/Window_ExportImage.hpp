#pragma once

#include "GUI/PopupWindow/_PopupWindow_WithConfirmationWarning.hpp"

#include "GUI/Widget/WidthHeightRatioPicker.hpp"
#include "GUI/Widget/FilepathPicker.hpp"
class Instance;

class Window_ExportImage : public PopupWindow_WithConfirmationWarning {
public:
	Window_ExportImage(Instance& instance);
	~Window_ExportImage() = default;

	void Show() override;
private:
	void OnConfirmation() override;
	bool WarnIf() override;
	std::string WarningMessage() override;

private:
	Instance& I;
	GUIwidget_WidthHeightRatioPicker m_widthHeightRatioPicker;
	GUIwidget_FilepathPicker m_filepathPicker;
};