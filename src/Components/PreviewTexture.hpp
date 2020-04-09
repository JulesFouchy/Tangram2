#pragma once

#include "Debugging/glException.hpp"

namespace Cmp {
struct Texture {
	Texture() = default; // required by registry.snapshot()
	void Delete() { // called by registry.on_destroy<Cmp::Texture>
		//spdlog::error("Delete Texture {}", id);
		glDeleteTextures(1, &id);
	}

	Texture(unsigned int width, unsigned int height,
			GLint GLpixelInternalFormat = GL_RGBA8, GLenum GLpixelFormat = GL_RGBA, GLenum GLpixelType = GL_UNSIGNED_BYTE, GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE)
		: width(width), height(height), aspectRatio((float)width / height), id(0),
		  m_GLpixelInternalFormat(GLpixelInternalFormat), m_GLpixelFormat(GLpixelFormat), m_GLpixelType(GLpixelType), m_interpolationMode(interpolationMode), m_wrapMode(wrapMode)
	{
		createTexture();
	}

	unsigned int width;
	unsigned int height;
	float aspectRatio;
	unsigned int id;
private:
	GLint m_GLpixelInternalFormat;
	GLenum m_GLpixelFormat;
	GLenum m_GLpixelType;
	GLint m_interpolationMode;
	GLint m_wrapMode;

	void createTexture(){
		// Gen texture
		GLCall(glGenTextures(1, &id));
		GLCall(glBindTexture(GL_TEXTURE_2D, id));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_interpolationMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_interpolationMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapMode));
		// Set size
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_GLpixelInternalFormat, width, height, 0, m_GLpixelFormat, m_GLpixelType, nullptr));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		//spdlog::warn("Create Texture {}", id);
	}

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(width, height, aspectRatio, 
			m_GLpixelInternalFormat, m_GLpixelFormat, m_GLpixelType, m_interpolationMode, m_wrapMode);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(width, height, aspectRatio,
			m_GLpixelInternalFormat, m_GLpixelFormat, m_GLpixelType, m_interpolationMode, m_wrapMode);
		createTexture();
	}
};
}