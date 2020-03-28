#pragma once

#include <entt/entt.hpp>

class ShaderSystem {
public:
	ShaderSystem() = delete;

	static entt::entity Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath);
};