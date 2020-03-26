#pragma once

#include "Components/History.hpp"

#include <entt/entt.hpp>

class HistoryManager {
public:
	HistoryManager() = delete;

	static void MoveBackward(entt::registry& R, entt::entity e);
	static void MoveForward(entt::registry& R, entt::entity e);

	static void RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev);
	static void RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev, glm::mat3 curr);

private:
	static inline Cmp::History* GetHistory(entt::registry& R, entt::entity e) { return R.try_get<Cmp::History>(e); }
};