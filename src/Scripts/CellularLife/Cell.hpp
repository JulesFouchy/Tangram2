#pragma once

class Cell {
	friend class CellularLife;
public:
	Cell() = default;
	Cell(entt::entity e);

	void move(entt::registry& R, float dt, float maxRadius);
	void applyForce(float dt, const glm::vec2& force);
	void applyDamping(float dt, float dampingCoef);

	glm::vec2 getPosition(entt::registry& R);
	void setPosition(entt::registry& R, const glm::vec2& newPos);

private:
	entt::entity m_entity;
	glm::vec2 m_speed;
};