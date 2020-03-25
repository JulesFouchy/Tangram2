#pragma once

class Settings {
friend class RenderSystem;
public:
	Settings();
	~Settings() = default;

	void ImGuiParameters();

	static inline float GetMIN_OPACITY_TO_GRAB_LAYER() { return m_MIN_OPACITY_TO_GRAB_LAYER; }

private:
	inline float getGamma() { return m_gamma; }
private:
	float m_gamma;
	static float m_MIN_OPACITY_TO_GRAB_LAYER;
};