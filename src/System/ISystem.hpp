#pragma once

#include <entt/entt.hpp>

class ISystem {
public:
	virtual void update() = 0;

protected:
	ISystem(entt::registry& registry) 
		: m_registry(registry)
	{}
	~ISystem() = default;

	entt::registry& m_registry;
};
