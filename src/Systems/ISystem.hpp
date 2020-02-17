#pragma once

#include <entt/entt.hpp>

class Instance;

class ISystem {
protected:
	ISystem(Instance& instance)
		: I(instance)
	{}
	~ISystem() = default;

	Instance& I;
};
