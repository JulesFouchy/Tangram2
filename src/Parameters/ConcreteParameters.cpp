#include "ConcreteParameters.hpp"

#include "Core/MustRecomputeTexture.hpp"

#include <imgui/imgui.h>

#include <glad/glad.h>
#include <entt/entt.hpp>

#include "Debugging/Log.hpp"

// Float1
FloatParameter::FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool FloatParameter::ImGuiItem() {
	return ImGui::SliderFloat(m_name.c_str(), &m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
bool FloatParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void FloatParameter::sendToShader() {
	glUniform1f(m_glUniformLocation, m_val);
}
void* FloatParameter::getValuePtr() {
	return &m_val;
}
size_t FloatParameter::getHash() {
	return GetHash(m_name, "float");
}
// Float2
Float2Parameter::Float2Parameter(int glUniformLocation, const std::string& name, const glm::vec2& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float2Parameter::ImGuiItem() {
	return ImGui::SliderFloat2(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
bool Float2Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Float2Parameter::sendToShader() {
	glUniform2f(m_glUniformLocation, m_val.x, m_val.y);
}
void* Float2Parameter::getValuePtr() {
	return &m_val;
}
size_t Float2Parameter::getHash() {
	return GetHash(m_name, "vec2");
}
// Float3
Float3Parameter::Float3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float3Parameter::ImGuiItem() {
	return ImGui::SliderFloat3(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
bool Float3Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Float3Parameter::sendToShader() {
	glUniform3f(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
void* Float3Parameter::getValuePtr() {
	return &m_val;
}
size_t Float3Parameter::getHash() {
	return GetHash(m_name, "vec3");
}
// Float4
Float4Parameter::Float4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
bool Float4Parameter::ImGuiItem() {
	return ImGui::SliderFloat4(m_name.c_str(), (float*)&m_val, m_minVal, m_maxVal, m_format.c_str(), m_power);
}
bool Float4Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Float4Parameter::sendToShader() {
	glUniform4f(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}
void* Float4Parameter::getValuePtr() {
	return &m_val;
}
size_t Float4Parameter::getHash() {
	return GetHash(m_name, "vec4");
}
// Color3
Color3Parameter::Color3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_flags(flags)
{}
bool Color3Parameter::ImGuiItem() {
	return ImGui::ColorPicker3(m_name.c_str(), (float*)&m_val, m_flags);
}
bool Color3Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Color3Parameter::sendToShader() {
	glUniform3f(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
void* Color3Parameter::getValuePtr() {
	return &m_val;
}
size_t Color3Parameter::getHash() {
	return GetHash(m_name, "vec3");
}
// Color4
Color4Parameter::Color4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_flags(flags)
{}
bool Color4Parameter::ImGuiItem() {
	return ImGui::ColorPicker4(m_name.c_str(), (float*)&m_val, m_flags);
}
bool Color4Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Color4Parameter::sendToShader() {
	glUniform4f(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}
void* Color4Parameter::getValuePtr() {
	return &m_val;
}
size_t Color4Parameter::getHash() {
	return GetHash(m_name, "vec4");
}
// Point2D
#include "Shapes/ShapeFactory.hpp"
#include "Core/GetPosition.hpp"
Point2DParameter::Point2DParameter(entt::registry& R, entt::entity parentLayer, int glUniformLocation, const std::string& name, const glm::vec2& val)
	: Parameter(glUniformLocation, name), m_valBeforeEdit(val), m_R(R)
{
	m_entityPoint = ShapeFactory::CreatePoint2D(R, parentLayer, val);
	R.assign<entt::tag<"SaveMeInTheHistoryOfMyParentsParameters"_hs>>(m_entityPoint);
}
Point2DParameter::~Point2DParameter() {
	m_R.destroy(m_entityPoint);
}
glm::vec2 Point2DParameter::getVal() {
	return TNG::GetPosition(m_R, m_entityPoint);
}
bool Point2DParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	return true;
}
void Point2DParameter::sendToShader() {
	glm::vec2 val = getVal();
	glUniform2f(m_glUniformLocation, val.x, val.y);
}
void* Point2DParameter::getValuePtr() {
	return nullptr;
}
size_t Point2DParameter::getHash() {
	return GetHash(m_name, "point2D");
}
// Bool
BoolParameter::BoolParameter(int glUniformLocation, const std::string& name, bool val)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val)
{}
bool BoolParameter::ImGuiItem() {
	return ImGui::Checkbox(m_name.c_str(), &m_val);
}
bool BoolParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void BoolParameter::sendToShader() {
	glUniform1i(m_glUniformLocation, m_val);
}
void* BoolParameter::getValuePtr() {
	return &m_val;
}
size_t BoolParameter::getHash() {
	return GetHash(m_name, "bool");
}
// Int
IntParameter::IntParameter(int glUniformLocation, const std::string& name, int val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool IntParameter::ImGuiItem() {
	return ImGui::SliderInt(m_name.c_str(), &m_val, m_minVal, m_maxVal);
}
bool IntParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void IntParameter::sendToShader() {
	glUniform1i(m_glUniformLocation, m_val);
}
void* IntParameter::getValuePtr() {
	return &m_val;
}
size_t IntParameter::getHash() {
	return GetHash(m_name, "int");
}
// Int2
Int2Parameter::Int2Parameter(int glUniformLocation, const std::string& name, const glm::ivec2& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int2Parameter::ImGuiItem() {
	return ImGui::SliderInt2(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
bool Int2Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Int2Parameter::sendToShader() {
	glUniform2i(m_glUniformLocation, m_val.x, m_val.y);
}
void* Int2Parameter::getValuePtr() {
	return &m_val;
}
size_t Int2Parameter::getHash() {
	return GetHash(m_name, "ivec2");
}
// Int3
Int3Parameter::Int3Parameter(int glUniformLocation, const std::string& name, const glm::ivec3& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int3Parameter::ImGuiItem() {
	return ImGui::SliderInt3(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
bool Int3Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Int3Parameter::sendToShader() {
	glUniform3i(m_glUniformLocation, m_val.x, m_val.y, m_val.z);
}
void* Int3Parameter::getValuePtr() {
	return &m_val;
}
size_t Int3Parameter::getHash() {
	return GetHash(m_name, "ivec3");
}
// Int4
Int4Parameter::Int4Parameter(int glUniformLocation, const std::string& name, const glm::ivec4& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
bool Int4Parameter::ImGuiItem() {
	return ImGui::SliderInt4(m_name.c_str(), (int*)&m_val, m_minVal, m_maxVal);
}
bool Int4Parameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	bool b = ImGuiItem();
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		history.beginUndoGroup();
		auto val = m_val;
		auto prevVal = m_valBeforeEdit;
		history.addAction(Action(
			[&R, layer, this, val]() {
				this->m_val = val;
				this->m_valBeforeEdit = val;
				TNG::MustRecomputeTexture(R, layer);
			},
			[&R, layer, this, prevVal]() {
				this->m_val = prevVal;
				this->m_valBeforeEdit = prevVal;
				TNG::MustRecomputeTexture(R, layer);
			}
			));
		history.endUndoGroup();
		m_valBeforeEdit = m_val; // ready for next edit
	}
	return b;
}
void Int4Parameter::sendToShader() {
	glUniform4i(m_glUniformLocation, m_val.x, m_val.y, m_val.z, m_val.w);
}
void* Int4Parameter::getValuePtr() {
	return &m_val;
}
size_t Int4Parameter::getHash() {
	return GetHash(m_name, "ivec4");
}