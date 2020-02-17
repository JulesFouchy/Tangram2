#pragma once

#include "ISystem.hpp"

class GUISystem : public ISystem {
public:
	GUISystem(Instance& instance);
	~GUISystem() = default;

	static inline bool ShowGUI() { return s_bShowGUI; }
private:
friend class Instance;
	void render();
friend class InputState_Rest;
	static inline void ToggleOnOff() { s_bShowGUI = !s_bShowGUI; }

private:
	static bool s_bShowGUI;
};