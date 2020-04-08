#pragma once

#include <entt/entt.hpp>

class GUISystem {
public:
	GUISystem() = default;
	~GUISystem() = default;

	static inline bool ShowGUI() { return s_bShowGUI; }
private:
	void LayersWindow(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& selectedLayer, entt::entity drawingBoardEntity);
friend class Instance;
	void render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& rSelectedLayer, entt::entity drawingBoardEntity);
friend class InputState_Rest;
	static inline void ToggleOnOff() { s_bShowGUI = !s_bShowGUI; }

private:
	static bool s_bShowGUI;
};