#pragma once

#include <string>

class Parameter {
public:
	Parameter() = default;
	Parameter(int glUniformLocation, const std::string& name)
		: m_glUniformLocation(glUniformLocation), m_name(name)
	{}
	virtual bool ImGui() = 0;
	virtual void sendToShader() = 0;

protected:
	std::string m_name;

	int m_glUniformLocation;
};