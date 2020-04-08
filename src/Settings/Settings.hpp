#pragma once

class Settings {
friend class RenderSystem;
public:
	Settings();
	~Settings() = default;

	void ImGuiParameters();

	static inline float GetMIN_OPACITY_TO_GRAB_LAYER() { return m_MIN_OPACITY_TO_GRAB_LAYER; }
	static inline unsigned int GetSELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS() { return m_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS; }

private:
	inline float getGamma() { return m_gamma; }
private:
	float m_gamma;
	static float m_MIN_OPACITY_TO_GRAB_LAYER;
	static unsigned int m_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS;
};