#include "ConcreteParameters.hpp"

#include "Core/MustRecomputeTexture.hpp"

#include "Shapes/ShapeFactory.hpp"
#include "Core/GetPosition.hpp"

#include "Systems/ShaderSystem.hpp"

// Float1
FloatParameter::FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
void FloatParameter::setVal(float val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void FloatParameter::copyValueTo(Parameter* paramPtr) {
	((FloatParameter*)paramPtr)->setVal(m_val);
}
size_t FloatParameter::getHash() {
	return GetHash(m_name, "float");
}
// Float2
Float2Parameter::Float2Parameter(int glUniformLocation, const std::string& name, const glm::vec2& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
void Float2Parameter::setVal(const glm::vec2& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Float2Parameter::copyValueTo(Parameter* paramPtr) {
	((Float2Parameter*)paramPtr)->setVal(m_val);
}
size_t Float2Parameter::getHash() {
	return GetHash(m_name, "vec2");
}
// Float3
Float3Parameter::Float3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
void Float3Parameter::setVal(const glm::vec3& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Float3Parameter::copyValueTo(Parameter* paramPtr) {
	((Float3Parameter*)paramPtr)->setVal(m_val);
}
size_t Float3Parameter::getHash() {
	return GetHash(m_name, "vec3");
}
// Float4
Float4Parameter::Float4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, float minVal, float maxVal, const std::string& format, float power)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal), m_format(format), m_power(power)
{}
void Float4Parameter::setVal(const glm::vec4& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Float4Parameter::copyValueTo(Parameter* paramPtr) {
	((Float4Parameter*)paramPtr)->setVal(m_val);
}
size_t Float4Parameter::getHash() {
	return GetHash(m_name, "vec4");
}
// Color3
Color3Parameter::Color3Parameter(int glUniformLocation, const std::string& name, const glm::vec3& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_flags(flags)
{}
void Color3Parameter::setVal(const glm::vec3& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
bool Color3Parameter::ImGuiItem() {
	return ImGui::ColorEdit3(m_name.c_str(), (float*)&m_val, m_flags);
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
void Color3Parameter::copyValueTo(Parameter* paramPtr) {
	((Color3Parameter*)paramPtr)->setVal(m_val);
}
size_t Color3Parameter::getHash() {
	return GetHash(m_name, "color3");
}
// Color4
Color4Parameter::Color4Parameter(int glUniformLocation, const std::string& name, const glm::vec4& val, ImGuiColorEditFlags flags)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_flags(flags)
{}
void Color4Parameter::setVal(const glm::vec4& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
bool Color4Parameter::ImGuiItem() {
	return ImGui::ColorEdit4(m_name.c_str(), (float*)&m_val, m_flags);
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
void Color4Parameter::copyValueTo(Parameter* paramPtr) {
	((Color4Parameter*)paramPtr)->setVal(m_val);
}
size_t Color4Parameter::getHash() {
	return GetHash(m_name, "color4");
}
// Point2D
Point2DParameter::Point2DParameter(entt::registry& R, entt::entity parentLayer, int glUniformLocation, const std::string& name, const glm::vec2& val)
	: Parameter(glUniformLocation, name), m_valBeforeEdit(val), m_R(&R)
{
	m_entityPoint = ShapeFactory::CreatePoint2D(R, parentLayer, val);
	R.assign<entt::tag<"SaveMeInTheHistoryOfMyParentsParameters"_hs>>(m_entityPoint);
}
Point2DParameter::~Point2DParameter() {
	m_R->destroy(m_entityPoint);
}
glm::vec2 Point2DParameter::getVal() {
	return TNG::GetPosition(*m_R, m_entityPoint);
}
bool Point2DParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	return false;
}
void Point2DParameter::sendToShader() {
	glm::vec2 val = getVal();
	glUniform2f(m_glUniformLocation, val.x, val.y);
}
void Point2DParameter::copyValueTo(Parameter* paramPtr) {
	m_R->replace<Cmp::TransformMatrix>(((Point2DParameter*)paramPtr)->m_entityPoint, m_R->get<Cmp::TransformMatrix>(m_entityPoint).val());
}
size_t Point2DParameter::getHash() {
	return GetHash(m_name, "point2D");
}
// List of Points2D
ListOfPoints2DParameter::ListOfPoints2DParameter(entt::registry& R, entt::entity parentLayer, const std::string& name, int size)
	: Parameter(-1, name), m_size(size)
{
	m_list.reserve(size);
	for (int i = 0; i < size; ++i)
		addPoint2D(R, parentLayer);
}
void ListOfPoints2DParameter::addPoint2D(entt::registry& R, entt::entity parentLayer, const glm::vec2& val) {
	m_list.emplace_back(R, parentLayer, -1, m_name + "[" + std::to_string(m_list.size()) + "]", val);
}
bool ListOfPoints2DParameter::ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) {
	ImGui::InputInt(("Nb of " + m_name).c_str(), &m_size);
	bool b = ImGui::IsItemDeactivatedAfterEdit();
	if (m_size < 1)
		m_size = 1;
	if (b) {
		ShaderSystem::CompileShaderAndUpdateParametersList(R, layer);
	}
	return false;
}
void ListOfPoints2DParameter::sendToShader() {
	for (Point2DParameter& param : m_list)
		param.sendToShader();
}
void ListOfPoints2DParameter::copyValueTo(Parameter* paramPtr) {
	ListOfPoints2DParameter* ptr = (ListOfPoints2DParameter*)paramPtr;
	size_t size = std::min(ptr->m_list.size(), m_list.size());
	for (int i = 0; i < size; ++i) {
		ptr->m_list[i].m_name = m_list[i].m_name;
		m_list[i].copyValueTo(&ptr->m_list[i]);
	}
}
size_t ListOfPoints2DParameter::getHash() {
	return GetHash(m_name, "listOfPoints2D");
}
void ListOfPoints2DParameter::computeUniformLocation(int shaderID) {
	for (Point2DParameter& param : m_list)
		param.computeUniformLocation(shaderID);
}
void ListOfPoints2DParameter::fillListOfDefinesInShader(std::vector<std::pair<std::string, std::string>>& modifyFromTo) {
	modifyFromTo.push_back({ "u." + m_name + ".size", std::to_string(m_list.size()) });
}
// Bool
BoolParameter::BoolParameter(int glUniformLocation, const std::string& name, bool val)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val)
{}
void BoolParameter::setVal(bool val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void BoolParameter::copyValueTo(Parameter* paramPtr) {
	((BoolParameter*)paramPtr)->setVal(m_val);
}
size_t BoolParameter::getHash() {
	return GetHash(m_name, "bool");
}
// Int
IntParameter::IntParameter(int glUniformLocation, const std::string& name, int val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
void IntParameter::setVal(int val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void IntParameter::copyValueTo(Parameter* paramPtr) {
	((IntParameter*)paramPtr)->setVal(m_val);
}
size_t IntParameter::getHash() {
	return GetHash(m_name, "int");
}
// Int2
Int2Parameter::Int2Parameter(int glUniformLocation, const std::string& name, const glm::ivec2& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
void Int2Parameter::setVal(const glm::ivec2& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Int2Parameter::copyValueTo(Parameter* paramPtr) {
	((Int2Parameter*)paramPtr)->setVal(m_val);
}
size_t Int2Parameter::getHash() {
	return GetHash(m_name, "ivec2");
}
// Int3
Int3Parameter::Int3Parameter(int glUniformLocation, const std::string& name, const glm::ivec3& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
void Int3Parameter::setVal(const glm::ivec3& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Int3Parameter::copyValueTo(Parameter* paramPtr) {
	((Int3Parameter*)paramPtr)->setVal(m_val);
}
size_t Int3Parameter::getHash() {
	return GetHash(m_name, "ivec3");
}
// Int4
Int4Parameter::Int4Parameter(int glUniformLocation, const std::string& name, const glm::ivec4& val, int minVal, int maxVal)
	: Parameter(glUniformLocation, name), m_val(val), m_valBeforeEdit(val), m_minVal(minVal), m_maxVal(maxVal)
{}
void Int4Parameter::setVal(const glm::ivec4& val) {
	m_val = val;
	m_valBeforeEdit = val;
}
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
void Int4Parameter::copyValueTo(Parameter* paramPtr) {
	((Int4Parameter*)paramPtr)->setVal(m_val);
}
size_t Int4Parameter::getHash() {
	return GetHash(m_name, "ivec4");
}