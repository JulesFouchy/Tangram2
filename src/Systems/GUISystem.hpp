#pragma once

#include "ISystem.hpp"

class GUISystem : public ISystem {
public:
	GUISystem(Instance& instance);
	~GUISystem() = default;

private:
friend class Instance;
	void render();
friend class InputState_Rest;
	inline void toggleOnOff() { m_bShowGUI = !m_bShowGUI; }

private:
	bool m_bShowGUI;
};