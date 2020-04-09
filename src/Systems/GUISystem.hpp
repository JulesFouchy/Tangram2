#pragma once

class GUISystem {
public:
	GUISystem() = delete;

	static void Render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& rSelectedLayer);
	static inline bool ShouldShowGUI() { return s_bShowGUI; }
	static inline void ToggleOnOff() { s_bShowGUI = !s_bShowGUI; }

private:
	static void LayersWindow(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& selectedLayer);

private:
	static bool s_bShowGUI;
};