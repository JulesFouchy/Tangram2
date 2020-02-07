#include "RenderSystem.hpp"

#include "Instance.hpp"

#include "Debugging/glException.hpp"
#include <glad/glad.h>

unsigned int RenderSystem::m1to1QuadVBOid;
unsigned int RenderSystem::m1to1QuadVAOid;
Shader RenderSystem::s_shaderUV   ("res/shaders/default.vert", "res/shaders/showTexture.frag", false);
Shader RenderSystem::s_shaderPoint("res/shaders/default.vert", "res/shaders/point.frag", false);

RenderSystem::RenderSystem(Instance& instance)
	: ISystem(instance)
{}

void RenderSystem::render() {
	renderQuad({ I.drawingBoardId() }, s_shaderUV);
	renderQuad(I.layersManager().m_layersOrdered, s_shaderUV);
	I.registry().view<entt::tag<"Point2D"_hs>>().each([this](auto entity, auto& tag) {
		renderSquare({ entity }, s_shaderPoint);
		});
}


void RenderSystem::_renderQuad(entt::entity e, Shader& shader, std::function<glm::mat3(entt::entity)> getMatrix) {
	shader.bind();
	shader.setUniformMat3f("u_mat", getMatrix(e));
	glBindVertexArray(m1to1QuadVAOid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderSystem::renderQuad(const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this](entt::entity e) { return I.getMatrixPlusAspectRatio(e); });
}

void RenderSystem::renderSquare(const std::vector<entt::entity>& list, Shader& shader) {
	for (const entt::entity& entity : list)
		_renderQuad(entity, shader, [this](entt::entity e) { return I.getMatrix(e); });
}

void RenderSystem::Initialize() {
	s_shaderUV.compile();
	s_shaderPoint.compile();
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