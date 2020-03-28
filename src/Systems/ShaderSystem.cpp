#include "ShaderSystem.hpp"

entt::entity ShaderSystem::Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	entt::entity e = R.create();
	R.assign<Cmp::Shader>(e, vertexFilepath, fragmentFilepath);
	return e;
}

void ShaderSystem::FillParametersList(entt::registry& R, entt::entity shaderEntity, std::vector<entt::entity>& parametersList) {

}