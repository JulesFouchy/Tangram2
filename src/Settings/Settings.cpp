#include "Settings.hpp"

#include "Systems/RenderSystem.hpp"

float Settings::s_MIN_OPACITY_TO_GRAB_LAYER = 0.5f;
unsigned int Settings::s_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS = 115;
unsigned int Settings::s_PREVIEW_SIZE_IN_PX = 1000;

Settings::Settings()
	: m_gamma(1.6f)
{
	RenderSystem::SetGamma(m_gamma);
}

void Settings::ImGuiParameters() {
	if (ImGui::SliderFloat("Gamma", &m_gamma, 1.0f, 3.0f)) {
		RenderSystem::SetGamma(m_gamma);
	}
	ImGui::SliderFloat("Min Opacity to Grab Layer", &s_MIN_OPACITY_TO_GRAB_LAYER, 0.0f, 255.0f); 
	{
		static unsigned int m = 0;
		static unsigned int M = 250;
		ImGui::SliderScalar("Selection Safety : Min Time in ms before Translating a Layer", ImGuiDataType_U32, &s_SELECT_SAFETY__MIN_TIME_BEFORE_MOVING_LAYER_IN_MS, &m, &M);
	}
}