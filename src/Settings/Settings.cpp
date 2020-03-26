#include "Settings.hpp"

#include <imgui/imgui.h>

#include "Systems/RenderSystem.hpp"

float Settings::m_MIN_OPACITY_TO_GRAB_LAYER = 0.5f;
int Settings::m_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS = 115;

Settings::Settings()
	: m_gamma(1.6f)
{
	RenderSystem::SetGamma(m_gamma);
}

void Settings::ImGuiParameters() {
	if (ImGui::SliderFloat("Gamma", &m_gamma, 1.0f, 3.0f)) {
		RenderSystem::SetGamma(m_gamma);
	}
	ImGui::SliderFloat("Min Opacity to Grab Layer", &m_MIN_OPACITY_TO_GRAB_LAYER, 0.0f, 255.0f);
	ImGui::SliderInt("Selection Safety : Min Time in ms before Translating a Layer", &m_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS, 0, 250);
}