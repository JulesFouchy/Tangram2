#pragma once

#include "System/ISystem.hpp"

#include "OpenGL/Shader.hpp"

#include <glm/glm.hpp>

class RenderSystem : public ISystem {
public:
	RenderSystem(entt::registry& registry);
	static void Initialize();
	~RenderSystem() = default;
	static void ShutDown();

	void update() override;

private:
	glm::mat3 getMatrix(entt::entity id, bool bIncludeRatio = true);

private:
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
	static Shader shader;
};
