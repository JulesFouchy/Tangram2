#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

class Cell {
public:
	Cell(entt::entity e);

	void move(entt::registry& R, float dt);
	void applyForce(float dt, const glm::vec2& force);
	void applyDamping(float dt, float dampingCoef);

	void setPosition(entt::registry& R, const glm::vec2& newPos);

private:
	glm::vec2 getPosition(entt::registry& R);

private:
	entt::entity m_entity;
	glm::vec2 m_speed;
};