#pragma once

#include "System/ISystem.hpp"

#include "OpenGL/Shader.hpp"

#include <vector>
#include <functional>

class RenderSystem : public ISystem {
public:
	RenderSystem(Instance& instance);
	~RenderSystem() = default;
	static void Initialize();
	static void ShutDown();

	void render();

private:
	void _renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix);
	void renderQuad(const std::vector<entt::entity>& list, Shader& shader);
	void renderSquare(const std::vector<entt::entity>& list, Shader& shader);

private:
	static Shader s_shaderUV;
	static Shader s_shaderPoint;
	static Shader s_shaderPolygon;
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
};
