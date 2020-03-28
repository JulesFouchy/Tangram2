#pragma once

#include "Parameter.hpp"

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

class FloatParameter : public Parameter {
public:
	FloatParameter() = default;
	FloatParameter(int glUniformLocation, const std::string& name, float val, float minVal = 0.0f, float maxVal = 1.0f, const std::string& format = "%.3f", float power = 1.0f);
	~FloatParameter() = default;

	bool ImGui() override;
	void sendToShader() override;

private:	
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(m_name, m_val, m_minVal, m_maxVal, m_format, m_power);
	}

private:
	float m_val;
	float m_minVal;
	float m_maxVal;
	std::string m_format;
	float m_power;
};

CEREAL_REGISTER_TYPE(FloatParameter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Parameter, FloatParameter)