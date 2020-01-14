#include "RenderSystem.hpp"

#include <glad/glad.h>
#include "Debugging/glException.hpp"

#include "Components/Translation.hpp"
#include "Components/Scale.hpp"

#include <gtx/matrix_transform_2d.hpp>

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
	shader.bind();
	m_registry.view<Cmp::Translation, Cmp::Scale>().each([](auto entity, auto& translation, auto& scale) {
		glm::mat3 model = glm::translate(glm::mat3(1.0f), translation.val);
		model = glm::scale(model, scale.val);
		shader.setUniformMat3f("u_modelMat", model);
		glBindVertexArray(m1to1QuadVAOid);
		glDrawArrays(GL_TRIANGLES,0 , 6);
	});
}