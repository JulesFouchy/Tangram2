#include "Cell.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

Cell::Cell(entt::entity e)
	: m_entity(e), m_speed(0.0f)
{
	int N = 10;
	int i = ((int)e) % N;
	float agl = i * 6.28 / N;
	float vel = 3.0f;
	m_speed.x = vel * cos(agl);
	m_speed.y = vel * sin(agl);
}

void Cell::move(entt::registry& R, float dt) {
	glm::vec2 pos = getPosition(R);
	pos += m_speed * dt;
	setPosition(R, pos);
}

void Cell::applyForce(float dt, const glm::vec2& force) {
	m_speed += force * dt;
}

void Cell::applyDamping(float dt, float dampingCoef) {
	applyForce(dt, -m_speed * dampingCoef);
}

void Cell::setPosition(entt::registry& R, const glm::vec2& newPos) {
	TNG::SetPosition(R, m_entity, newPos);
}
glm::vec2 Cell::getPosition(entt::registry& R) {
	return TNG::GetPosition(R, m_entity);
}