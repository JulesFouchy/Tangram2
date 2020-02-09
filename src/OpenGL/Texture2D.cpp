#include "Texture2D.hpp"

#include "Helper/File.hpp"

#include "Debugging/Log.hpp"
#include "Debugging/glException.hpp"

std::vector<bool> Texture2D::isSlotUsed;
Shader Texture2D::s_shader("res/shaders/texture.vert", "res/shaders/texture.frag", false);

Texture2D::Texture2D(GLint GLpixelInternalFormat, GLenum GLpixelFormat, GLenum GLpixelType, GLint interpolationMode, GLint wrapMode)
	: m_bytesPerPixel(BytesPerPixel(GLpixelFormat)), m_GLpixelInternalFormat(GLpixelInternalFormat), m_GLpixelFormat(GLpixelFormat), m_GLpixelType(GLpixelType),
	  m_textureSlot(-1)
{
	// Gen texture
	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture2D::Texture2D(Texture2D&& other) noexcept
	: m_width(other.m_width), m_height(other.m_height), m_aspectRatio(other.m_aspectRatio),
	m_bytesPerPixel(other.m_bytesPerPixel),
	m_GLpixelInternalFormat(other.m_GLpixelInternalFormat),
	m_GLpixelFormat(other.m_GLpixelFormat),
	m_GLpixelType(other.m_GLpixelType),
	m_textureID(other.m_textureID),
	m_textureSlot(-1)
{
	other.m_textureID = -1;
}

void Texture2D::setData(int width, int height, void* pixels) {
	m_width = width;
	m_height = height;
	m_aspectRatio = (float)m_width / m_height;

	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_GLpixelInternalFormat, m_width, m_height, 0, m_GLpixelFormat, m_GLpixelType, pixels));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
void Texture2D::setSize(int width, int height) {
	setData(width, height, nullptr);
}
void Texture2D::Initialize() {
	for (int i = 0; i < MAX_NB_TEXTURES; ++i)
		isSlotUsed.push_back(false);
	s_shader.compile();
}

Texture2D::~Texture2D() {
	GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture2D::bind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::attachToSlotAndBind() {
	// find slot to bind to
	if (m_textureSlot == -1) {
		auto it = std::find(isSlotUsed.cbegin(), isSlotUsed.cend(), false);
		if (it == isSlotUsed.cend())
			spdlog::error("[Texture2D::attachToSlotAndBind] no empty slot found !");
		else
			m_textureSlot = std::distance(isSlotUsed.cbegin(), it);
	}
	else {
		spdlog::error("[Texture2D::attachToSlotAndBind] not correctly unbound");
	}
	isSlotUsed[m_textureSlot] = true;
	// attach to slot
	GLCall(glActiveTexture(GL_TEXTURE0 + m_textureSlot));
	// bind
	bind();
}

void Texture2D::detachAndUnbind() {
	if (m_textureSlot > -1) {
		isSlotUsed[m_textureSlot] = false;
		m_textureSlot = -1;
		//
		GLCall(glActiveTexture(GL_TEXTURE0));
		unbind();
	}
	else {
		spdlog::warn("[Texture2D::detachAndUnbind] trying to detachAndUnbind a texture that wasn't bound");
	}
}

unsigned int Texture2D::BytesPerPixel(GLenum GLpixelFormat) {
	switch (GLpixelFormat) {
	case GL_RGBA:
		return 4;
	case GL_BGRA:
		return 4;
	case GL_RGB:
		return 3;
	case GL_BGR:
		return 3;
	case GL_RG:
		return 2;
	case GL_RED:
		return 1;
	case GL_DEPTH_COMPONENT:
		return 1;
	default:
		spdlog::error("[Texture2D::BytesPerPixel] Unknown pixel format : {}", (int)GLpixelFormat);
	}
}