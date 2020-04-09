#pragma once

#include <fstream>

#include "Components/Shader.hpp"

#include "Parameters/Parameter.hpp"

class ShaderSystem {
public:
	ShaderSystem() = delete;

	static entt::entity Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	static void CompileShaderAndUpdateParametersList(entt::registry& R, entt::entity layer);
	static void UpdateParametersList(entt::registry& R, entt::entity parentLayer, entt::entity shaderEntity, std::vector<std::shared_ptr<Parameter>>& parametersList);
	static void TryReloadShader(entt::registry& R, entt::entity layer);
	static void ComputeUniformLocations(entt::registry& R, entt::entity layerWithAShader);

private:
	static inline Cmp::Shader& GetShaderCmp(entt::registry& R, entt::entity shader) { return R.get<Cmp::Shader>(shader); }

	static int GetUniformLocation(int glShaderID, const std::string& parameterName);
	// Parsing
	static void GoToFirstLineOfStructParameters(std::ifstream& stream);
	static std::shared_ptr<Parameter> CreateParameterFromLine(entt::registry& R, entt::entity parentLayer, const std::string& line, int glShaderID, const std::vector<std::shared_ptr<Parameter>>& prevList);
	template <typename T>
	static T ReadValue(const std::string& line, const std::string& variableName) {
		size_t pos = MyString::FindCaseInsensitive(line, variableName, line.find("//")); // we start the search at the comment (//) to prevent confusion with the name of the variable
		if (pos != std::string::npos) {
			pos += variableName.length();
			return MyString::ReadNumbersAt<T>(line, pos);
		}
		spdlog::error("[ShaderSystem::ReadValue] Couldn't read value \"{}\" at line \"{}\"", variableName, line);
		return T(0);
	}
};