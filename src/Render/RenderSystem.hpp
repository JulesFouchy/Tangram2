#pragma once

#include "System/ISystem.hpp"

#include "OpenGL/Shader.hpp"

class RenderSystem : public ISystem {
public:
	RenderSystem(entt::registry& registry);
	static void Initialize();
	~RenderSystem() = default;
	static void ShutDown();

	void update() override;

private:
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
	static Shader shader;
};
