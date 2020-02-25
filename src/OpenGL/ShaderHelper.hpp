#pragma once

#include <string>

namespace ShaderHelper {
	std::string parseFile(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
}