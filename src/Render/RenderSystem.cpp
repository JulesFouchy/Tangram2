#include "RenderSystem.hpp"

#include <glad/glad.h>
#include "Debugging/glException.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

#include "Helper/DisplayInfos.hpp"

#include "Components/Translation.hpp"
#include "Components/Scale.hpp"
#include "Components/AspectRatio.hpp"

#include "Components/Parent.hpp"

#include "App.hpp"

unsigned int RenderSystem::m1to1QuadVBOid;
unsigned int RenderSystem::m1to1QuadVAOid;
Shader RenderSystem::shader("res/shaders/showTexture.vert", "res/shaders/showTexture.frag", false);

RenderSystem::RenderSystem(entt::registry& registry)
	: ISystem(registry)
{}

void RenderSystem::Initialize() {
	shader.compile();
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

void RenderSystem::update() {
	renderPreviewTextures({ App::Get().m_drawingBoardId });
	renderPreviewTextures(App::Get().m_layersManager.m_layersOrdered);
}

void RenderSystem::renderPreviewTextures(const std::vector<entt::entity>& list) {
	shader.bind();
	for (const entt::entity& entity : list){
		shader.setUniformMat3f("u_mat", getMatrix(entity));
		glBindVertexArray(m1to1QuadVAOid);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

glm::mat3 RenderSystem::getMatrix(entt::entity id, bool bIncludeRatio) {
	glm::mat3 model = glm::translate(glm::mat3(1.0f), m_registry.get<Cmp::Translation>(id).val);
	model = glm::scale(model, glm::vec2(m_registry.get<Cmp::Scale>(id).val));
	if (bIncludeRatio)
		model = glm::scale(model, glm::vec2(m_registry.get<Cmp::AspectRatio>(id).val, 1.0f));
	Cmp::Parent* parent = m_registry.try_get<Cmp::Parent>(id);
	if (parent)
		return getMatrix(parent->id, false) * model;
	else
		return DisplayInfos::Matrix() * model;
}