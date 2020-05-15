#pragma once

#include "Helper/Random.hpp"

class Cell {
	friend class CellularLife;
public:
	Cell() = default;
	Cell(Rand& rand, entt::entity e);

	void move(entt::registry& R, float dt, float maxRadius);
	void applyForce(float dt, const glm::vec2& force);
	void applyDamping(float dt, float dampingCoef);

	glm::vec2 getPosition(entt::registry& R);
	void setPosition(entt::registry& R, const glm::vec2& newPos);

	inline unsigned int getTypeID() { return m_typeID; }

private:
	int m_frameCount = 0;
	entt::entity m_entity;
	glm::vec2 m_speed;
	unsigned int m_typeID;
	glm::vec2 m_dir;
	Rand m_rand;
};