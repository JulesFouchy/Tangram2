#pragma once

class Instance;

class IInputState {
public:
	IInputState(Instance& instance);
	virtual ~IInputState() = default;

	virtual void update() {}

	virtual void onLeftClicDown(entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) {}
	virtual void onLeftClicUp() {}
	virtual void onRightClicDown() {}
	virtual void onRightClicUp() {}
	virtual void onWheelDown() {}
	virtual void onWheelUp() {}
	virtual void onKeyDown(SDL_Scancode key) {}
	virtual void onKeyUp(SDL_Scancode key) {}
	virtual void onWheelScroll(float dl) {}

protected:
	Instance& I;
};