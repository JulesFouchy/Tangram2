#pragma once

class Instance;

class ISystem {
protected:
	ISystem(Instance& instance)
		: I(instance)
	{}
	~ISystem() = default;

	Instance& I;
};
