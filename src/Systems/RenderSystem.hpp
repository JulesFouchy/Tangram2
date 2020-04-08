#pragma once

#include <entt/entt.hpp>
#include <vector>
#include <functional>

#include "OpenGL/Shader.hpp"

#include "Components/PreviewTexture.hpp"
#include "OpenGL/RenderBuffer.hpp"

#include "Settings/Settings.hpp"

class RenderSystem {
public:
	RenderSystem() = default;
	~RenderSystem() = default;
	static void Initialize();
	static void ShutDown();

	void render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity selectedLayer);
	void checkTexturesToRecompute(entt::registry& R);

	void exportImage(entt::registry& R, const std::vector<entt::entity>& layersOrdered, unsigned int width, unsigned int height, const std::string& filepath);

	inline unsigned int previewWidth() { return Settings::GetPREVIEW_SIZE_IN_PX(); }
	inline unsigned int previewHeight() { return Settings::GetPREVIEW_SIZE_IN_PX(); }

private:
	void _renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix);
	void renderQuad(entt::registry& R, const std::vector<entt::entity>& list, Shader& shader);
	void renderSquare(entt::registry& R, const std::vector<entt::entity>& list, Shader& shader);
	void renderTextures(entt::registry& R, const std::vector<entt::entity>& list);

	void blendTextures(entt::registry& R, const std::vector<entt::entity>& sources, Cmp::Texture& destination);
	void blendTextures(Cmp::Texture& source, Cmp::Texture& destination);

	void beginBlendTexture(Cmp::Texture& destination);
	void doBlendTexture(Cmp::Texture& source);
	void endBlendTexture();

	void drawFragment(entt::registry& R, entt::entity e);
	//void drawTest(entt::entity e);
	//void drawPolygon(entt::entity e);

	void clear();
	void drawFullscreen();

	void beginComputeTexture(entt::registry& R, entt::entity e);
	void endComputeTexture();
	void computeTexture_Fragment(entt::registry& R, entt::entity e);
	//void computeTexture_Test(entt::entity e);
	//void computeTexture_Polygon(entt::entity e);

friend class LayersManager;
friend class Instance;
	inline void setRenderTarget_Texture(Cmp::Texture& texture) { m_renderBuffer.setRenderTarget_Texture(texture); }
	inline void setRenderTarget_Screen() { m_renderBuffer.setRenderTarget_Screen(); }

friend class Settings;
	static void SetGamma(float gamma);

private:
	RenderBuffer m_renderBuffer;

	static Shader s_shaderTest;
	static Shader s_shaderDrawingBoard;
	static Shader s_shaderPoint;
	static Shader s_shaderTexture;
	static Shader s_shaderBlend;
	static unsigned int m1to1QuadVBOid;
	static unsigned int m1to1QuadVAOid;
};
