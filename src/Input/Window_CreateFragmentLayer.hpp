#pragma once

#include "GUI/PopupWindow/_PopupWindow.hpp"

#include "GUI/Widget/FilepathPicker.hpp"
class Instance;

class Window_CreateFragmentLayer : public PopupWindow {
public:
	Window_CreateFragmentLayer(Instance& instance);
	~Window_CreateFragmentLayer() = default;

	void Show() override;
private:
	void OnConfirmation() override;

private:
	Instance& I;
	GUIwidget_FilepathPicker m_filepathPicker;
};