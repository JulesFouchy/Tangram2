#pragma once

#include "OpenGL/ShaderHelper.hpp"

#include "Debugging/Log.hpp"

#include <cereal/access.hpp>

#include "Debugging/glException.hpp"
#include <glad/glad.h>

namespace Cmp {
struct Shader {
	unsigned int id;
	std::string vertexFilepath;
	std::string fragmentFilepath;

	Shader() = default; // required by registry.snapshot()
	void Delete()
	{   // called by registry.on_destroy<Cmp::Shader>
		spdlog::error("Delete Shader {}", id);
		GLCall(glDeleteProgram(id));
	}

	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
		: vertexFilepath(vertexFilepath), fragmentFilepath(fragmentFilepath)
	{ createShader(); }

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(vertexFilepath, fragmentFilepath);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(vertexFilepath, fragmentFilepath);
		createShader();
	}

	void createShader() {
		spdlog::info("[Creating Shader Component] " + vertexFilepath + " & " + fragmentFilepath);
		id = glCreateProgram();
		unsigned int vs = ShaderHelper::compileShader(GL_VERTEX_SHADER, ShaderHelper::parseFile(vertexFilepath));
		unsigned int fs = ShaderHelper::compileShader(GL_FRAGMENT_SHADER, ShaderHelper::parseFile(fragmentFilepath));

		glAttachShader(id, vs);
		glAttachShader(id, fs);
		glLinkProgram(id);
		glValidateProgram(id);

		glDeleteShader(vs);
		glDeleteShader(fs);
		Log::separationLine();
	}
};
}