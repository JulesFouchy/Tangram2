#pragma once

#include <entt/entt.hpp>

class GUISystem {
public:
	GUISystem() = delete;

	static void Render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& rSelectedLayer, entt::entity drawingBoardEntity);
	static inline bool ShouldShowGUI() { return s_bShowGUI; }
	static inline void ToggleOnOff() { s_bShowGUI = !s_bShowGUI; }

private:
	static void LayersWindow(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& selectedLayer, entt::entity drawingBoardEntity);

private:
	static bool s_bShowGUI;
};