#include "RenderBuffer.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Debugging/Log.hpp"
#include "Debugging/glException.hpp"
#include <glad/glad.h>

RenderBuffer::RenderBuffer() {
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

RenderBuffer::~RenderBuffer() {
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void RenderBuffer::setRenderTarget_Texture(Cmp::Texture& texture) {
	//assert(texture.width == m_previewWidth && texture.height == m_previewHeight);
	// Bind Framebuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	// Set viewport
	GLCall(glViewport(0, 0, m_previewWidth, m_previewHeight));
	// Bind and Attach texture
	GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0));
}

void RenderBuffer::setRenderTarget_Screen(){
	// Unbind FrameBuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	// Set viewport
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
}