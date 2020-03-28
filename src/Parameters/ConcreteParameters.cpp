#include "ConcreteParameters.hpp"

#include <imgui/imgui.h>

#include <glad/glad.h>

// Float1
FloatParameter::FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool FloatParameter::ImGui() {
	return ImGui::SliderFloat(m_name.c_str(), &m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
void FloatParameter::sendToShader() {
	glUniform1f(m_glUniformLocation, m_val);
}
// Float2
Float2Parameter::Float2Parameter(int glUniformLocation, const std::string& name, const glm::vec2& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float2Parameter::ImGui() {
	return ImGui::SliderFloat2(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
void Float2Parameter::sendToShader() {
	glUniform2f(m_glUniformLocation, m_val.x, m_val.y);
}
// Float3
Float3Parameter::Float3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float3Parameter::ImGui() {
	return ImGui::SliderFloat3(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
void Float3Parameter::sendToShader() {
	glUniform3f(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
// Float4
Float4Parameter::Float4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float4Parameter::ImGui() {
	return ImGui::SliderFloat4(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
void Float4Parameter::sendToShader() {
	glUniform4f(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}