#pragma once

#include "System/ISystem.hpp"

#include "OpenGL/Shader.hpp"

#include "Components/PreviewTexture.hpp"
#include "OpenGL/RenderBuffer.hpp"

#include <vector>
#include <functional>

class RenderSystem : public ISystem {
public:
	RenderSystem(Instance& instance);
	~RenderSystem() = default;
	static void Initialize();
	static void ShutDown();

	void render();

	inline unsigned int previewWidth() { return m_renderBuffer.previewWidth(); }
	inline unsigned int previewHeight() { return m_renderBuffer.previewHeight(); }

private:
	void _renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix);
	void renderQuad(const std::vector<entt::entity>& list, Shader& shader);
	void renderSquare(const std::vector<entt::entity>& list, Shader& shader);
	void renderPreviewTexture(const std::vector<entt::entity>& list);
	void renderPolygon(const std::vector<entt::entity>& vertices, float smoothMin);

friend class LayersManager;
	inline void setRenderTarget_Texture(Cmp::Texture& texture) { m_renderBuffer.setRenderTarget_Texture(texture); }
	inline void setRenderTarget_Screen() { m_renderBuffer.setRenderTarget_Screen(); }

private:
	RenderBuffer m_renderBuffer;

	static Shader s_shaderTest;
	static Shader s_shaderDrawingBoard;
	static Shader s_shaderPoint;
	static Shader s_shaderPolygon;
	static Shader s_shaderTexture;
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
};
