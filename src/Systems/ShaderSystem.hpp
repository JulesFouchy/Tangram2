#pragma once

#include <entt/entt.hpp>
#include <fstream>

#include "Components/Shader.hpp"

class ShaderSystem {
public:
	ShaderSystem() = delete;

	static entt::entity Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	static void FillParametersList(entt::registry& R, entt::entity shaderEntity, std::vector<entt::entity>& parametersList);

private:
	static inline Cmp::Shader& GetShaderCmp(entt::registry& R, entt::entity shader) { return R.get<Cmp::Shader>(shader); }

	// Parsing
	static void GoToFirstLineOfStructParameters(std::ifstream& stream);
	static entt::entity CreateParameterFromLine(entt::registry& R, const std::string& line);
};