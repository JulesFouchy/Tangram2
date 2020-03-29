#pragma once

#include <string>

class Parameter {
	friend class ShaderSystem;
public:
	Parameter() = default;
	Parameter(int glUniformLocation, const std::string& name)
		: m_glUniformLocation(glUniformLocation), m_name(name)
	{}
	virtual bool ImGui() = 0;
	virtual void sendToShader() = 0;
	virtual size_t getHash() = 0;

protected:
	std::string m_name;

	int m_glUniformLocation;
};