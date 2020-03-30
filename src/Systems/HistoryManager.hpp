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
	static Cmp::History* GetActiveHistory(entt::registry& R, entt::entity e);
	static Cmp::History* GetTransformHistory(entt::registry& R, entt::entity e);
	static Cmp::History& GetParametersHistory(entt::registry& R, entt::entity layer);
};