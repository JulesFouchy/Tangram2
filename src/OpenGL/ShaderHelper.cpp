#include "ShaderHelper.hpp"

#include <fstream>
#include <iostream>

#include "Helper/String.hpp"

std::string ShaderHelper::parseFile(const std::string& filepath, const std::vector<std::pair<std::string, std::string>>& modifyFromTo) {
	std::ifstream stream(filepath);
	if (!stream.is_open()) {
		spdlog::warn("Failed to open file |{}|", filepath);
		//m_bCreatedSuccessfully = false;
		return "";
	}

	std::string str = "";
	std::string tempLine = "";
	while (getline(stream, tempLine)) {
		str += tempLine + '\n';
	}
	stream.close();
	for (const auto& pair : modifyFromTo)
		MyString::ReplaceAll(str, pair.first, pair.second);
	return str;
}

unsigned int ShaderHelper::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//Debug
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		//m_bCreatedSuccessfully = false;
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		spdlog::warn("Failed to compile {} {}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex"), "shader");
		spdlog::warn(message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}
