#pragma once

#include <entt/entt.hpp>
#include <fstream>

#include "Components/Shader.hpp"

#include "Parameters/Parameter.hpp"

#include <memory>

class ShaderSystem {
public:
	ShaderSystem() = delete;

	static entt::entity Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	static void FillParametersList(entt::registry& R, entt::entity shaderEntity, std::vector<std::unique_ptr<Parameter>>& parametersList);

private:
	static inline Cmp::Shader& GetShaderCmp(entt::registry& R, entt::entity shader) { return R.get<Cmp::Shader>(shader); }

	// Parsing
	static void GoToFirstLineOfStructParameters(std::ifstream& stream);
	static std::unique_ptr<Parameter> CreateParameterFromLine(entt::registry& R, const std::string& line, int glShaderID);
	template <typename T>
	static T ReadValue(const std::string& line, const std::string& variableName) {
		size_t pos = MyString::FindCaseInsensitive(line, variableName);
		if (pos != std::string::npos) {
			pos += variableName.length();
			return ConvertStringTo<T>(line, pos);
		}
		spdlog::error("[ShaderSystem::ReadValue] Couldn't read value \"{}\" at line \"{}\"", variableName, line);
		return T(0);
	}
	template <typename T>
	static T ConvertStringTo(const std::string& str, size_t pos = 0);
};