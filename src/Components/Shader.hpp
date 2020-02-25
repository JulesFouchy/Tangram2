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

	void bind() {
		GLCall(glUseProgram(id));
	}

	void setUniform1i(const std::string& uniformName, int v) {
		glUniform1i(getUniformLocation(uniformName), v);
	}
	void setUniform1f(const std::string& uniformName, float v) {
		glUniform1f(getUniformLocation(uniformName), v);
	}
	void setUniform2f(const std::string& uniformName, const glm::vec2& v) {
		glUniform2f(getUniformLocation(uniformName), v.x, v.y);
	}
	void setUniform3f(const std::string& uniformName, const glm::vec3& v) {
		glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z);
	}
	void setUniform4f(const std::string& uniformName, const glm::vec4& v) {
		glUniform4f(getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
	}
	void setUniformMat3f(const std::string& uniformName, const glm::mat3& mat) {
		glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
	}
	void setUniformMat4f(const std::string& uniformName, const glm::mat4& mat) {
		glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
	}

private:
	std::unordered_map<std::string, int> m_UniformLocationCache;

private:

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

	int getUniformLocation(const std::string& uniformName) {
		if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end()) {
			return m_UniformLocationCache[uniformName];
		}

		int location = glGetUniformLocation(id, uniformName.c_str());
		//if (location == -1) {
		//	spdlog::warn("[Shader] uniform '{}' doesn't exist !", name);
		//}
		m_UniformLocationCache[uniformName] = location;
		return location;
	}

	// Serialization
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
};
}