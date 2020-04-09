#pragma once

class Settings {
friend class RenderSystem;
public:
	Settings();
	~Settings() = default;

	void ImGuiParameters();

	static inline float GetMIN_OPACITY_TO_GRAB_LAYER() { return s_MIN_OPACITY_TO_GRAB_LAYER; }
	static inline unsigned int GetSELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS() { return s_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS; }
	static inline unsigned int GetPREVIEW_SIZE_IN_PX() { return s_PREVIEW_SIZE_IN_PX; }

private:
	inline float getGamma() { return m_gamma; }
private:
	float m_gamma;
	static float s_MIN_OPACITY_TO_GRAB_LAYER;
	static unsigned int s_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS;
	static unsigned int s_PREVIEW_SIZE_IN_PX;
};