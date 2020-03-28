#pragma once

#include "Parameter.hpp"

#include <glm/glm.hpp>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <imgui/imgui.h>

class FloatParameter : public Parameter {
public:
	FloatParameter() = default;
	FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~FloatParameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	float m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:	
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val, m_minVal, m_maxVal, m_format, m_power);
	}
};

class Float2Parameter : public Parameter {
public:
	Float2Parameter() = default;
	Float2Parameter(int glUniformLocation, const std::string& name, const glm::vec2& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float2Parameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	glm::vec2 m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_minVal, m_maxVal, m_format, m_power);
	}
};

class Float3Parameter : public Parameter {
public:
	Float3Parameter() = default;
	Float3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float3Parameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	glm::vec3 m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_minVal, m_maxVal, m_format, m_power);
	}
};

class Float4Parameter : public Parameter {
public:
	Float4Parameter() = default;
	Float4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float4Parameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	glm::vec4 m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_minVal, m_maxVal, m_format, m_power);
	}
};

class Color3Parameter : public Parameter {
public:
	Color3Parameter() = default;
	Color3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags = 0);
	~Color3Parameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	glm::vec3 m_val;
	ImGuiColorEditFlags m_flags;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_flags);
	}
};

class Color4Parameter : public Parameter {
public:
	Color4Parameter() = default;
	Color4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, ImGuiColorEditFlags flags = 0);
	~Color4Parameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	glm::vec4 m_val;
	ImGuiColorEditFlags m_flags;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_flags);
	}
};

class BoolParameter : public Parameter {
public:
	BoolParameter() = default;
	BoolParameter(int glUniformLocation, const std::string& name, bool val);
	~BoolParameter() = default;
	bool ImGui() override;
	void sendToShader() override;
private:
	bool m_val;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_name, m_val);
	}
};

CEREAL_REGISTER_TYPE(FloatParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, FloatParameter)

CEREAL_REGISTER_TYPE(Float2Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Float2Parameter)

CEREAL_REGISTER_TYPE(Float3Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Float3Parameter)

CEREAL_REGISTER_TYPE(Float4Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Float4Parameter)

CEREAL_REGISTER_TYPE(Color3Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Color3Parameter)

CEREAL_REGISTER_TYPE(Color4Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Color4Parameter)

CEREAL_REGISTER_TYPE(BoolParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, BoolParameter)