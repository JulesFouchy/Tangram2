#include "FloatParameter.hpp"

#include <imgui/imgui.h>

#include <glad/glad.h>

FloatParameter::FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}

bool FloatParameter::ImGui() {
	return ImGui::SliderFloat(m_name.c_str(), &m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}

void FloatParameter::sendToShader() {
	glUniform1f(m_glUniformLocation, m_val);
}