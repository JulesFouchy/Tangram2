#include "Cell.hpp"

#include "CellTypes.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

Cell::Cell(Rand& rand, entt::entity e)
	: m_entity(e), m_speed(0.0f), m_typeID(rand.Int(NB_TYPES))
{
	/*int N = 10;
	int i = ((int)e) % N;
	float agl = i * 6.28 / N;
	float vel = 3.0f;
	m_speed.x = vel * cos(agl);
	m_speed.y = vel * sin(agl);*/
	float angle = m_rand.Float() * 6.28;
	m_dir = glm::vec2(cos(angle), sin(angle));
}

void Cell::move(entt::registry& R, float dt, float maxRadius) {
	m_frameCount++;

	float thresh = 0.18f * ((200 - m_frameCount) / 200.0f);

	float mag2 = glm::length(m_speed);
	if (mag2 > thresh)
		m_speed *= thresh / mag2;

	glm::vec2 speed = m_speed + m_dir * 0.1f;

	float mag = glm::length(speed);
	if (mag > thresh * 2.0f)
		speed *= thresh * 2.0f / mag;

	glm::vec2 pos = getPosition(R);
	pos += speed * dt;
	float d = glm::length(pos);
	if (d > maxRadius) {
		pos *= maxRadius / d;
		if (m_rand.Float() < 0.005) {
			spdlog::info("dsf");
			float angle = m_rand.Float() * 6.28;
			m_dir = glm::vec2(cos(angle), sin(angle));
		}
	}
	// avoid text zone
	glm::vec2 avoidZone = glm::vec2(-0.25, -0.15);
	glm::vec2 delta = pos - avoidZone;
	float dist = glm::length(delta);
	if (dist < 0.25) {
		pos += delta / dist * (0.25f - dist);		
		if (m_rand.Float() < 0.005) {
			float angle = m_rand.Float() * 6.28;
			m_dir = glm::vec2(cos(angle), sin(angle));
		}
	}
	//
	setPosition(R, pos);
}

void Cell::applyForce(float dt, const glm::vec2& force) {
	m_speed += force * dt;
}

void Cell::applyDamping(float dt, float dampingCoef) {
	applyForce(dt, -m_speed * dampingCoef);
	//applyForce(dt, -m_speed * m_speed * 50.0f);
}

glm::vec2 Cell::getPosition(entt::registry& R) {
	return TNG::GetPosition(R, m_entity);
}
void Cell::setPosition(entt::registry& R, const glm::vec2& newPos) {
	TNG::SetPosition(R, m_entity, newPos);
}