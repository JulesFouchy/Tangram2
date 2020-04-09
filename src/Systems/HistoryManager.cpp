#include "HistoryManager.hpp"

#include "Components/Parent.hpp"
#include "Components/Parameters.hpp"

#include "Components/TransformMatrix.hpp"

Cmp::History* HistoryManager::GetActiveHistory(entt::registry& R, entt::entity e) {
	if (R.has<entt::tag<"ActiveHistoryIsParameter"_hs>>(e))
		return &GetParametersHistory(R, e);
	return GetTransformHistory(R, e);
}

Cmp::History* HistoryManager::GetTransformHistory(entt::registry& R, entt::entity e) {
	return R.try_get<Cmp::History>(e);
}
Cmp::History& HistoryManager::GetParametersHistory(entt::registry& R, entt::entity layer) {
	return R.get<Cmp::Parameters>(layer).history;
}

void HistoryManager::MoveBackward(entt::registry& R, entt::entity e) {
	Cmp::History* h = GetActiveHistory(R, e);
	if (h)
		h->moveBackward();
}

void HistoryManager::MoveForward(entt::registry& R, entt::entity e) {
	Cmp::History* h = GetActiveHistory(R, e);
	if (h)
		h->moveForward();
}

void HistoryManager::RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev) {
	RecordTransform(R, e, prev, R.get<Cmp::TransformMatrix>(e).val());
}

void HistoryManager::RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev, glm::mat3 curr) {
	Cmp::History* h = GetTransformHistory(R, e);
	if (!h && R.has<entt::tag<"SaveMeInTheHistoryOfMyParentsParameters"_hs>>(e))
		h = &GetParametersHistory(R, R.get<Cmp::Parent>(e).id);
	if (h) {
		h->beginUndoGroup();
		h->addAction(Action(
			// Do
			[&R, e, curr]() {
				Cmp::TransformMatrix& transform = R.get<Cmp::TransformMatrix>(e);
				R.replace<Cmp::TransformMatrix>(e, curr);
			},
			// Undo
			[&R, e, prev]() {
				Cmp::TransformMatrix& transform = R.get<Cmp::TransformMatrix>(e);
				R.replace<Cmp::TransformMatrix>(e, prev);
			}
		));
		h->endUndoGroup();
	}
#ifndef NDEBUG
	if (!h)
		spdlog::warn("[HistoryManager::RecordTransform] : entity has no history");
#endif
}