#include "RenderSystem.hpp"

#include "Instance.hpp"

#include "GUISystem.hpp"

#include "Components/Vertices.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/ParametersList.hpp"
#include "Components/Parent.hpp"
#include "Components/Shader.hpp"
#include "Components/ShaderReference.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Debugging/glException.hpp"
#include <glad/glad.h>

#include "stb_image/stb_image_write.h"

#include "Helper/String.hpp"
#include "Helper/DisplayInfos.hpp"

unsigned int RenderSystem::m1to1QuadVBOid;
unsigned int RenderSystem::m1to1QuadVAOid;
Shader RenderSystem::s_shaderTest         ("res/shaders/defaultDrawOnTexture.vert", "res/shaders/test.frag", false);
Shader RenderSystem::s_shaderDrawingBoard ("res/shaders/default.vert", "res/shaders/drawingBoard.frag", false);
Shader RenderSystem::s_shaderPoint        ("res/shaders/default.vert", "res/shaders/point.frag", false);
Shader RenderSystem::s_shaderTexture      ("res/shaders/default.vert", "res/shaders/texture.frag", false);
Shader RenderSystem::s_shaderBlend        ("res/shaders/blendDefault.vert", "res/shaders/blendDefault.frag", false);

RenderSystem::RenderSystem(Instance& instance)
	: ISystem(instance)
{}

void RenderSystem::render() {
	// Drawing Board
	renderQuad({ I.drawingBoardId() }, s_shaderDrawingBoard);
	// Layers
	Cmp::Texture& dbTexture = I.registry().get<Cmp::Texture>(I.drawingBoardId());
	setRenderTarget_Texture(dbTexture);
	clear();
	blendTextures(I.layersManager().m_layersOrdered, dbTexture);
	glEnable(GL_BLEND);
	renderTextures({ I.drawingBoardId() });
	glDisable(GL_BLEND);
	// Points2D
	if (GUISystem::ShowGUI()) {
		glEnable(GL_BLEND);
		I.registry().view<entt::tag<"Point2D"_hs>>().each([this](auto entity, auto& tag) {
			Cmp::Parent* parent = I.registry().try_get<Cmp::Parent>(entity);
			if (!parent || parent->id == I.layersManager().selectedLayer())
				renderSquare({ entity }, s_shaderPoint);
			});
		glDisable(GL_BLEND);
	}
}

void RenderSystem::exportImage(unsigned int width, unsigned int height, const std::string& filepath) {
	spdlog::info("[Exporting as] " + filepath);
	std::string fileExtension = MyString::GetFileExtension(filepath);
	//
	Cmp::Texture renderTexture(width, height);
	Cmp::Texture tmpTexture(width, height);
	setRenderTarget_Texture(renderTexture);
	clear();
	for (entt::entity e : I.layersManager().m_layersOrdered) {
		setRenderTarget_Texture(tmpTexture);
		clear();		
		if (I.registry().has<entt::tag<"FragmentLayer"_hs>>(e)) {
			drawFragment(e);
		}
		else if (I.registry().has<entt::tag<"TestLayer"_hs>>(e)) {
			drawTest(e);
		}
		else if (I.registry().has<entt::tag<"Polygon"_hs>>(e)) {
			drawPolygon(e);
		}
		//
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

void RenderSystem::checkTexturesToRecompute() {
	I.registry().view<entt::tag<"FragmentLayer"_hs>, entt::tag<"MustRecomputeTexture"_hs>>().each([this](auto e, auto&, auto&) {
		computeTexture_Fragment(e);
		I.registry().remove<entt::tag<"MustRecomputeTexture"_hs>>(e);
		});
	I.registry().view<entt::tag<"Polygon"_hs>, entt::tag<"MustRecomputeTexture"_hs>>().each([this](auto e, auto&, auto&) {
		computeTexture_Polygon(e);
		I.registry().remove<entt::tag<"MustRecomputeTexture"_hs>>(e);
	});
	I.registry().view<entt::tag<"TestLayer"_hs>, entt::tag<"MustRecomputeTexture"_hs>>().each([this](auto e, auto&, auto&) {
		computeTexture_Test(e);
		I.registry().remove<entt::tag<"MustRecomputeTexture"_hs>>(e);
	});
}

void RenderSystem::_renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix) {
	shader.bind();
	shader.setUniformMat3f("u_mat", getMatrix(e));
	drawFullscreen();
}

void RenderSystem::renderQuad(const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this](entt::entity e) { return I.getMatrixPlusAspectRatio(e);  });
}

void RenderSystem::renderSquare(const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this](entt::entity e) { return I.getMatrix(e); });
}

void RenderSystem::renderTextures(const std::vector<entt::entity>& list) {
	s_shaderTexture.bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	s_shaderTexture.setUniform1i("u_TextureSlot", 0);
	// Loop
	for (entt::entity e : list) {
		// Matrix
		s_shaderTexture.setUniformMat3f("u_mat", I.getMatrixPlusAspectRatio(I.drawingBoardId()));
		// Texture
		Cmp::Texture& tex = I.registry().get<Cmp::Texture>(e);
		GLCall(glBindTexture(GL_TEXTURE_2D, tex.id));
		// Draw
		drawFullscreen();
	}
	// Unbind
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void RenderSystem::blendTextures(const std::vector<entt::entity>& sources, Cmp::Texture& destination) {
	beginBlendTexture(destination);
	for (entt::entity sourceEnt : sources) {
		Cmp::Texture& source = I.registry().get<Cmp::Texture>(sourceEnt);
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

void RenderSystem::drawFragment(entt::entity e) {
	Cmp::Shader& shader = I.registry().get<Cmp::Shader>(I.registry().get<Cmp::ShaderReference>(e).entityID);
	shader.bind();
	for (const auto& param : I.registry().get<Cmp::Parameters>(e).list) {
		param->sendToShader();
	}
	shader.setUniformMat3f("u_localTransformMat", I.getMatrixToTextureSpace(e));
	drawFullscreen();
}

void RenderSystem::drawTest(entt::entity e) {
	s_shaderTest.bind();
	s_shaderTest.setUniformMat3f("u_localTransformMat", I.getMatrixToTextureSpace(e));
	drawFullscreen();
}

void RenderSystem::drawPolygon(entt::entity e) {
	//s_shaderPolygon.bind();
	//s_shaderPolygon.setUniform1f("u_SmoothMin", 1.0f / I.registry().get<Cmp::SliderFloat>(I.registry().get<Cmp::Parameters>(e).list[0]).val);
	//s_shaderPolygon.setUniformMat3f("u_localTransformMat", I.getMatrixToTextureSpace(e));
	//int k = 0;
	//Cmp::Vertices& vertices = I.registry().get<Cmp::Vertices>(e);
	//for (entt::entity vertex : vertices.list) {
	//	s_shaderPolygon.setUniform2f("u_vertices[" + std::to_string(k) + "]", glm::vec2(glm::column(I.getLocalTransform(vertex), 2)));
	//	k++;
	//}
	//drawFullscreen();
}

void RenderSystem::drawFullscreen() {
	glBindVertexArray(m1to1QuadVAOid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderSystem::clear() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::beginComputeTexture(entt::entity e) {
	setRenderTarget_Texture(I.registry().get<Cmp::Texture>(e));
	clear();
}

void RenderSystem::endComputeTexture() {
	setRenderTarget_Screen();
}

void RenderSystem::computeTexture_Fragment(entt::entity e) {
	beginComputeTexture(e);
	drawFragment(e);
	endComputeTexture();
}

void RenderSystem::computeTexture_Test(entt::entity e) {
	beginComputeTexture(e);
	drawTest(e);
	endComputeTexture();
}

void RenderSystem::computeTexture_Polygon(entt::entity e) {
	beginComputeTexture(e);
	drawPolygon(e);
	endComputeTexture();
}

void RenderSystem::Initialize() {
	s_shaderTest.compile();
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