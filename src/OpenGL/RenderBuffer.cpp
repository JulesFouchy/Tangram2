#include "RenderBuffer.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Debugging/glException.hpp"

RenderBuffer::RenderBuffer() {
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

RenderBuffer::~RenderBuffer() {
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void RenderBuffer::setRenderTarget_Texture(Cmp::Texture& texture) {
	// Bind Framebuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	// Set viewport
	GLCall(glViewport(0, 0, texture.width, texture.height));
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