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
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(float val);
	bool ImGuiItem();
private:
	float m_val;
	float m_valBeforeEdit;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val, m_minVal, m_maxVal, m_format, m_power);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val, m_minVal, m_maxVal, m_format, m_power);
		m_valBeforeEdit = m_val;
	}
};

class Float2Parameter : public Parameter {
public:
	Float2Parameter() = default;
	Float2Parameter(int glUniformLocation, const std::string& name, const glm::vec2& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float2Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec2& val);
	bool ImGuiItem();
private:
	glm::vec2 m_val;
	glm::vec2 m_valBeforeEdit;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_minVal, m_maxVal, m_format, m_power);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_minVal, m_maxVal, m_format, m_power);
		m_valBeforeEdit = m_val;
	}
};

class Float3Parameter : public Parameter {
public:
	Float3Parameter() = default;
	Float3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float3Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec3& val);
	bool ImGuiItem();
private:
	glm::vec3 m_val;
	glm::vec3 m_valBeforeEdit;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_minVal, m_maxVal, m_format, m_power);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_minVal, m_maxVal, m_format, m_power);
		m_valBeforeEdit = m_val;
	}
};

class Float4Parameter : public Parameter {
public:
	Float4Parameter() = default;
	Float4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~Float4Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec4& val);
	bool ImGuiItem();
private:
	glm::vec4 m_val;
	glm::vec4 m_valBeforeEdit;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_minVal, m_maxVal, m_format, m_power);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_minVal, m_maxVal, m_format, m_power);
		m_valBeforeEdit = m_val;
	}
};

class Color3Parameter : public Parameter {
public:
	Color3Parameter() = default;
	Color3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags = 0);
	~Color3Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec3& val);
	bool ImGuiItem();
private:
	glm::vec3 m_val;
	glm::vec3 m_valBeforeEdit;
	ImGuiColorEditFlags m_flags;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_flags);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_flags);
		m_valBeforeEdit = m_val;
	}
};

class Color4Parameter : public Parameter {
public:
	Color4Parameter() = default;
	Color4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, ImGuiColorEditFlags flags = 0);
	~Color4Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec4& val);
	bool ImGuiItem();
private:
	glm::vec4 m_val;
	glm::vec4 m_valBeforeEdit;
	ImGuiColorEditFlags m_flags;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_flags);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_flags);
		m_valBeforeEdit = m_val;
	}
};

class Point2DParameter : public Parameter {
public:
	Point2DParameter() = default;
	Point2DParameter(entt::registry& R, entt::entity parentLayer, int glUniformLocation, const std::string & name, const glm::vec2& val);
	~Point2DParameter();
	bool ImGui(entt::registry& R, Cmp::History & history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::vec2& val);
	glm::vec2 getVal();
private:
	entt::entity m_entityPoint;
	glm::vec2 m_valBeforeEdit;
	entt::registry* m_R; // needed to delete the point entity upon destrutor call
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive & archive) {
		archive(m_name, m_entityPoint);
	}
	// Restore registry when loading project
	inline void initializeRegistry(entt::registry& R) override {
		m_R = &R;
	}
};

class ListOfPoints2DParameter : public Parameter {
public:
	ListOfPoints2DParameter() = default;
	ListOfPoints2DParameter(entt::registry& R, entt::entity parentLayer, const std::string& name, int size = 0);
	~ListOfPoints2DParameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
	void computeUniformLocation(int shaderID) override;
private:
	std::vector<Point2DParameter> m_list;

private:
	void addPoint2D(entt::registry& R, entt::entity parentLayer, const glm::vec2& val = glm::vec2(0.0f));
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_list);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_list);
	}
};

class BoolParameter : public Parameter {
public:
	BoolParameter() = default;
	BoolParameter(int glUniformLocation, const std::string& name, bool val);
	~BoolParameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(bool val);
	bool ImGuiItem();
private:
	bool m_val;
	bool m_valBeforeEdit;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val);
		m_valBeforeEdit = m_val;
	}
};

class IntParameter : public Parameter {
public:
	IntParameter() = default;
	IntParameter(int glUniformLocation, const std::string& name, int val, int minVal, int maxVal);
	~IntParameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(int val);
	bool ImGuiItem();
private:
	int m_val;
	int m_valBeforeEdit;
	int m_minVal;
	int m_maxVal;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val, m_minVal, m_maxVal);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val, m_minVal, m_maxVal);
		m_valBeforeEdit = m_val;
	}
};

class Int2Parameter : public Parameter {
public:
	Int2Parameter() = default;
	Int2Parameter(int glUniformLocation, const std::string& name, const glm::ivec2& val, int minVal, int maxVal);
	~Int2Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::ivec2& val);
	bool ImGuiItem();
private:
	glm::ivec2 m_val;
	glm::ivec2 m_valBeforeEdit;
	int m_minVal;
	int m_maxVal;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_minVal, m_maxVal);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_minVal, m_maxVal);
		m_valBeforeEdit = m_val;
	}
};

class Int3Parameter : public Parameter {
public:
	Int3Parameter() = default;
	Int3Parameter(int glUniformLocation, const std::string& name, const glm::ivec3& val, int minVal, int maxVal);
	~Int3Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::ivec3& val);
	bool ImGuiItem();
private:
	glm::ivec3 m_val;
	glm::ivec3 m_valBeforeEdit;
	int m_minVal;
	int m_maxVal;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_minVal, m_maxVal);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_minVal, m_maxVal);
		m_valBeforeEdit = m_val;
	}
};

class Int4Parameter : public Parameter {
public:
	Int4Parameter() = default;
	Int4Parameter(int glUniformLocation, const std::string& name, const glm::ivec4& val, int minVal, int maxVal);
	~Int4Parameter() = default;
	bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) override;
	void sendToShader() override;
	void copyValueTo(Parameter* paramPtr) override;
	size_t getHash() override;
private:
	void setVal(const glm::ivec4& val);
	bool ImGuiItem();
private:
	glm::ivec4 m_val;
	glm::ivec4 m_valBeforeEdit;
	int m_minVal;
	int m_maxVal;
private:
	// Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_minVal, m_maxVal);
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(m_name, m_val.x, m_val.y, m_val.z, m_val.w, m_minVal, m_maxVal);
		m_valBeforeEdit = m_val;
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

CEREAL_REGISTER_TYPE(Point2DParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Point2DParameter)

CEREAL_REGISTER_TYPE(ListOfPoints2DParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, ListOfPoints2DParameter)

CEREAL_REGISTER_TYPE(BoolParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, BoolParameter)

CEREAL_REGISTER_TYPE(IntParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, IntParameter)

CEREAL_REGISTER_TYPE(Int2Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Int2Parameter)

CEREAL_REGISTER_TYPE(Int3Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Int3Parameter)

CEREAL_REGISTER_TYPE(Int4Parameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, Int4Parameter)