#pragma once

#include "System/ISystem.hpp"

#include "OpenGL/Shader.hpp"

#include <vector>

class RenderSystem : public ISystem {
public:
	RenderSystem(Instance& instance);
	~RenderSystem() = default;
	static void Initialize();
	static void ShutDown();

	void render();

private:
	void renderPreviewTextures(const std::vector<entt::entity>& list);

private:
	static Shader shader;
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
};
