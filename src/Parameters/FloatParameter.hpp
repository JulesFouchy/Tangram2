#pragma once

#include "Parameter.hpp"

class FloatParameter : public Parameter {
public:
	FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~FloatParameter() = default;

	bool ImGui() override;
	void sendToShader() override;

private:
	float m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
};