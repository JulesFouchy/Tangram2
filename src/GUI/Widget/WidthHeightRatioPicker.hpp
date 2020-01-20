#pragma once

#include "Helper/Ratio.hpp"
#include <functional>

#include "RatioPicker.hpp"

enum WidthOrHeight {
	Width,
	Height
};

class GUIwidget_WidthHeightRatioPicker {
public:
	GUIwidget_WidthHeightRatioPicker(int buttonFlags = ButtonFlag_Square, Ratio initialRatio = Ratio(1, 1), std::function<void(void)> onUpdate = []() {});
	~GUIwidget_WidthHeightRatioPicker() = default;

	inline void ShowRatio() { m_ratioPicker.Show(); }
	void ShowWidthHeight();

	inline Ratio getRatio() const { return m_ratioPicker.getRatio(); }
	inline void setRatio(const Ratio& ratio) { m_ratioPicker.setRatio(ratio); }
	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }

private:
	void updateWidthOrHeight();

private:
	GUIwidget_RatioPicker m_ratioPicker;
	std::function<void(void)> m_onUpdate;
	unsigned int m_width;
	unsigned int m_height;
	WidthOrHeight m_lastModified;
};