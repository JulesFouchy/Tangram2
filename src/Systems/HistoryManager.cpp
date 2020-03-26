#include "HistoryManager.hpp"

#include "Components/Parent.hpp"

#include "Components/TransformMatrix.hpp"

Cmp::History* HistoryManager::GetHistory(entt::registry& R, entt::entity e) {
	Cmp::History* h = R.try_get<Cmp::History>(e);
	if (h)
		return h;
	// else
	Cmp::Parent* parent = R.try_get<Cmp::Parent>(e);
	if (parent)
		return GetHistory(R, parent->id);
	return nullptr;
}

void HistoryManager::MoveBackward(entt::registry& R, entt::entity e) {
	Cmp::History* h = GetHistory(R, e);
	if (h)
		h->moveBackward();
}

void HistoryManager::MoveForward(entt::registry& R, entt::entity e) {
	Cmp::History* h = GetHistory(R, e);
	if (h)
		h->moveForward();
}

void HistoryManager::RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev) {
	RecordTransform(R, e, prev, R.get<Cmp::TransformMatrix>(e).val());
}

void HistoryManager::RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev, glm::mat3 curr) {
	Cmp::History* h = GetHistory(R, e);
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