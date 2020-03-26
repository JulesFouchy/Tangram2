#include "HistoryManager.hpp"

#include "Components/TransformMatrix.hpp"

void HistoryManager::RecordTransform(entt::registry& R, entt::entity e, glm::mat3 prev, glm::mat3 next) {
	Cmp::History* h = GetHistory(R, e);
	if (h) {
		h->beginUndoGroup();
		h->addAction(Action(
			// Do
			[&R, e, next]() {
				Cmp::TransformMatrix& transform = R.get<Cmp::TransformMatrix>(e);
				R.replace<Cmp::TransformMatrix>(e, next);
			},
			// Undo
			[&R, e, prev]() {
				Cmp::TransformMatrix& transform = R.get<Cmp::TransformMatrix>(e);
				R.replace<Cmp::TransformMatrix>(e, prev);
			}
		));
		h->endUndoGroup();
	}
}