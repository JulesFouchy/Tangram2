#include "RenderSystem.hpp"

#include "Components/Parameters.hpp"
#include "Components/Parent.hpp"
#include "Components/Shader.hpp"
#include "Components/ShaderReference.hpp"

#include "GUISystem.hpp"

#include "Core/GetMatrix.hpp"
#include "Core/GetDrawingBoard.hpp"

#include "Debugging/glException.hpp"
#include <glad/glad.h>

#include "stb_image/stb_image_write.h"

#include "Helper/String.hpp"

unsigned int RenderSystem::m1to1QuadVBOid;
unsigned int RenderSystem::m1to1QuadVAOid;
Shader RenderSystem::s_shaderDrawingBoard ("res/shaders/default.vert", "res/shaders/drawingBoard.frag", false);
Shader RenderSystem::s_shaderPoint        ("res/shaders/default.vert", "res/shaders/point.frag", false);
Shader RenderSystem::s_shaderTexture      ("res/shaders/default.vert", "res/shaders/texture.frag", false);
Shader RenderSystem::s_shaderBlend        ("res/shaders/blendDefault.vert", "res/shaders/blendDefault.frag", false);

void RenderSystem::render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity selectedLayer) {
	// Drawing Board
	renderQuad(R, { TNG::GetDrawingBoard(R) }, s_shaderDrawingBoard);
	// Layers
	Cmp::Texture& dbTexture = R.get<Cmp::Texture>(TNG::GetDrawingBoard(R));
	setRenderTarget_Texture(dbTexture);
	clear();
	blendTextures(R, layersOrdered, dbTexture);
	glEnable(GL_BLEND);
	renderTextures(R, { TNG::GetDrawingBoard(R) });
	glDisable(GL_BLEND);
	// Points2D
	if (GUISystem::ShouldShowGUI()) {
		glEnable(GL_BLEND);
		R.view<entt::tag<"Point2D"_hs>>().each([this, &R, selectedLayer](auto entity, auto& tag) {
			Cmp::Parent* parent = R.try_get<Cmp::Parent>(entity);
			if (!parent || parent->id == selectedLayer)
				renderSquare(R, { entity }, s_shaderPoint);
			});
		glDisable(GL_BLEND);
	}
}

void RenderSystem::exportImage(entt::registry& R, const std::vector<entt::entity>& layersOrdered, unsigned int width, unsigned int height, const std::string& filepath) {
	spdlog::info("[Exporting as] " + filepath);
	std::string fileExtension = MyString::GetFileExtension(filepath);
	//
	Cmp::Texture renderTexture(width, height);
	Cmp::Texture tmpTexture(width, height);
	setRenderTarget_Texture(renderTexture);
	clear();
	for (entt::entity e : layersOrdered) {
		setRenderTarget_Texture(tmpTexture);
		clear();		
		if (R.has<entt::tag<"FragmentLayer"_hs>>(e)) {
			drawFragment(R, e);
		}
		blendTextures(tmpTexture, renderTexture);
	}
	// Save pixels
	setRenderTarget_Texture(renderTexture);
	unsigned char* data = new unsigned char[4 * width * height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	if (fileExtension == "png")
		stbi_write_png(filepath.c_str(), width, height, 4, data, 0);
	else if (fileExtension == "jpg")
		stbi_write_jpg(filepath.c_str(), width, height, 4, data, 100);
	else
		spdlog::error("Unknown file extension : |{}|", fileExtension);
	delete[] data;
	// Unbind
	setRenderTarget_Screen();
}

void RenderSystem::checkTexturesToRecompute(entt::registry& R) {
	R.view<entt::tag<"FragmentLayer"_hs>, entt::tag<"MustRecomputeTexture"_hs>>().each([this, &R](auto e, auto&, auto&) {
		computeTexture_Fragment(R, e);
		R.remove<entt::tag<"MustRecomputeTexture"_hs>>(e);
	});
}

void RenderSystem::_renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix) {
	shader.bind();
	shader.setUniformMat3f("u_mat", getMatrix(e));
	drawFullscreen();
}

void RenderSystem::renderQuad(entt::registry& R, const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this, &R](entt::entity e) { return TNG::GetMatrixPlusAspectRatio(R, e);  });
}

void RenderSystem::renderSquare(entt::registry& R, const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this, &R](entt::entity e) { return TNG::GetMatrix(R, e); });
}

void RenderSystem::renderTextures(entt::registry& R,  const std::vector<entt::entity>& list) {
	s_shaderTexture.bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	s_shaderTexture.setUniform1i("u_TextureSlot", 0);
	// Loop
	for (entt::entity e : list) {
		// Matrix
		s_shaderTexture.setUniformMat3f("u_mat", TNG::GetMatrixPlusAspectRatio(R, TNG::GetDrawingBoard(R)));
		// Texture
		Cmp::Texture& tex = R.get<Cmp::Texture>(e);
		GLCall(glBindTexture(GL_TEXTURE_2D, tex.id));
		// Draw
		drawFullscreen();
	}
	// Unbind
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void RenderSystem::blendTextures(entt::registry& R, const std::vector<entt::entity>& sources, Cmp::Texture& destination) {
	beginBlendTexture(destination);
	for (entt::entity sourceEnt : sources) {
		Cmp::Texture& source = R.get<Cmp::Texture>(sourceEnt);
		doBlendTexture(source);
	}
	endBlendTexture();
}

void RenderSystem::blendTextures(Cmp::Texture& source, Cmp::Texture& destination) {
	beginBlendTexture(destination);
	doBlendTexture(source);
	endBlendTexture();
}

void RenderSystem::beginBlendTexture(Cmp::Texture& destination) {
	setRenderTarget_Texture(destination);
	s_shaderBlend.bind();
	// Bind destination texture
	GLCall(glActiveTexture(GL_TEXTURE1));
	GLCall(glBindTexture(GL_TEXTURE_2D, destination.id));
	s_shaderBlend.setUniform1i("uDst", 1);
}
void RenderSystem::doBlendTexture(Cmp::Texture& source) {
	// Bind source texture
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, source.id));
	s_shaderBlend.setUniform1i("uSrc", 0);
	// Draw
	drawFullscreen();
}
void RenderSystem::endBlendTexture() {
	setRenderTarget_Screen();
}

void RenderSystem::drawFragment(entt::registry& R, entt::entity e) {
	Cmp::Shader& shader = R.get<Cmp::Shader>(R.get<Cmp::ShaderReference>(e).entityID);
	shader.bind();
	for (const auto& param : R.get<Cmp::Parameters>(e).list) {
		param->sendToShader();
	}
	shader.setUniformMat3f("u_localTransformMat", TNG::GetMatrixToTextureSpace(R, e));
	drawFullscreen();
}

void RenderSystem::drawFullscreen() {
	glBindVertexArray(m1to1QuadVAOid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderSystem::clear() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::beginComputeTexture(entt::registry& R, entt::entity e) {
	setRenderTarget_Texture(R.get<Cmp::Texture>(e));
	clear();
}

void RenderSystem::endComputeTexture() {
	setRenderTarget_Screen();
}

void RenderSystem::computeTexture_Fragment(entt::registry& R, entt::entity e) {
	beginComputeTexture(R, e);
	drawFragment(R, e);
	endComputeTexture();
}

void RenderSystem::Initialize() {
	s_shaderDrawingBoard.compile();
	s_shaderPoint.compile();
	s_shaderTexture.compile();
	s_shaderBlend.compile();
	GLCall(glGenVertexArrays(1, &m1to1QuadVAOid));
	GLCall(glGenBuffers(1, &m1to1QuadVBOid));
	// Vertices data
	float vertices[] = {
		//    pos      tex coords
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,

		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m1to1QuadVBOid));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	// Layout
	GLCall(glBindVertexArray(m1to1QuadVAOid));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m1to1QuadVBOid));
	// Positions
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
	// Texture coordinates
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void RenderSystem::ShutDown() {
	glDeleteBuffers(1, &m1to1QuadVBOid);
	glDeleteVertexArrays(1, &m1to1QuadVAOid);
}

void RenderSystem::SetGamma(float gamma) {
	s_shaderBlend.bind();
	s_shaderBlend.setUniform1f("u_gamma", gamma);
}