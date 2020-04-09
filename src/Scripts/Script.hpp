#pragma once

class Script {
public:
	Script() = default;
	virtual ~Script() = default;
	virtual void loopIteration(entt::registry& R, float dt) = 0;
	virtual void ImGui(entt::registry& R) = 0;
};