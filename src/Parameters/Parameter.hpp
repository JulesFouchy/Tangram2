#pragma once

#include <string>

#include "Components/History.hpp"

class Parameter {
	friend class ShaderSystem;
public:
	Parameter() = default;
	Parameter(int glUniformLocation, const std::string& name)
		: m_glUniformLocation(glUniformLocation), m_name(name)
	{}
	virtual ~Parameter() = default;
	virtual bool ImGui(entt::registry& R, Cmp::History& history, entt::entity layer) = 0;
	virtual void sendToShader() = 0;
	virtual void copyValueTo(Parameter* paramPtr) = 0;
	virtual size_t getHash() = 0;
	static size_t GetHash(const std::string& name, const std::string& type);

private:
	friend class Instance;
	virtual void initializeRegistry(entt::registry& R) {}; // required by Point2DParameter

protected:
	std::string m_name;

	int m_glUniformLocation;
};