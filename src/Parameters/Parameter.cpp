#include "Parameter.hpp"

size_t Parameter::GetHash(const std::string& name, const std::string& type) {
	return std::hash<std::string>{}(name + type);
}

void Parameter::computeUniformLocation(int shaderID) { 
	m_glUniformLocation = glGetUniformLocation(shaderID, ("u." + m_name).c_str());
}