#pragma once

#include <glm/glm.hpp>
#include "Debugging/glException.hpp"
#include <glad/glad.h>

namespace Cmp {
	struct PreviewTexture {
		unsigned int width;
		unsigned int height;
		float aspectRatio;
		unsigned int id;

		PreviewTexture(unsigned int width, unsigned int height,
					   GLint GLpixelInternalFormat = GL_RGBA8, GLenum GLpixelFormat = GL_RGBA, GLenum GLpixelType = GL_UNSIGNED_BYTE, GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE)
			: width(width), height(height), aspectRatio((float)width / height), id(0)
		{
			// Gen texture
			GLCall(glGenTextures(1, &id));
			GLCall(glBindTexture(GL_TEXTURE_2D, id));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
			// Set size
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat, width, height, 0, GLpixelFormat, GLpixelType, nullptr));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
	};
}