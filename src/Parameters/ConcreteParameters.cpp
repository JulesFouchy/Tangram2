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
// Color3
Color3Parameter::Color3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_flags(flags)
{}
bool Color3Parameter::ImGui() {
	return ImGui::ColorPicker3(m_name.c_str(), (float*)&m_val, m_flags);
}
void Color3Parameter::sendToShader() {
	glUniform3f(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
// Color4
Color4Parameter::Color4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_flags(flags)
{}
bool Color4Parameter::ImGui() {
	return ImGui::ColorPicker4(m_name.c_str(), (float*)&m_val, m_flags);
}
void Color4Parameter::sendToShader() {
	glUniform4f(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}
// Bool
BoolParameter::BoolParameter(int glUniformLocation, const std::string& name, bool val)
	: Parameter(glUniformLocation, name), m_val(val)
{}
bool BoolParameter::ImGui() {
	return ImGui::Checkbox(m_name.c_str(), &m_val);
}
void BoolParameter::sendToShader() {
	glUniform1i(m_glUniformLocation, m_val);
}
// Int
IntParameter::IntParameter(int glUniformLocation, const std::string& name, int val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool IntParameter::ImGui() {
	return ImGui::SliderInt(m_name.c_str(), &m_val, m_minVal, m_maxVal);
}
void IntParameter::sendToShader() {
	glUniform1i(m_glUniformLocation, m_val);
}
// Int2
Int2Parameter::Int2Parameter(int glUniformLocation, const std::string& name, const glm::ivec2& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int2Parameter::ImGui() {
	return ImGui::SliderInt2(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
void Int2Parameter::sendToShader() {
	glUniform2i(m_glUniformLocation, m_val.x, m_val.y);
}
// Int3
Int3Parameter::Int3Parameter(int glUniformLocation, const std::string& name, const glm::ivec3& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int3Parameter::ImGui() {
	return ImGui::SliderInt3(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
void Int3Parameter::sendToShader() {
	glUniform3i(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
// Int4
Int4Parameter::Int4Parameter(int glUniformLocation, const std::string& name, const glm::ivec4& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int4Parameter::ImGui() {
	return ImGui::SliderInt4(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
void Int4Parameter::sendToShader() {
	glUniform4i(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}