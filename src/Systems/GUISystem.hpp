#pragma once

#include "ISystem.hpp"

class GUISystem : public ISystem {
public:
	GUISystem(Instance& instance);
	~GUISystem() = default;

private:
friend class Instance;
	void render();
};