#include "GetMatrix.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"

#include "Helper/DisplayInfos.hpp"

#include "GetDrawingBoard.hpp"

#include "glm/gtx/matrix_transform_2d.hpp"

glm::mat3 TNG::GetLocalTransform(entt::registry& R, entt::entity e) {
	return R.get<Cmp::TransformMatrix>(e).val();
}

glm::mat3 TNG::GetMatrix(entt::registry& R, entt::entity e) {
	glm::mat3 model = GetLocalTransform(R, e);
	return DisplayInfos::Matrix() * GetParentModelMatrix(R, e) * model;
}

glm::mat3 TNG::GetMatrixPlusAspectRatio(entt::registry& R, entt::entity e) {
	glm::mat3 model = GetLocalTransform(R, e);
	Cmp::AspectRatio* ratio = R.try_get<Cmp::AspectRatio>(e);
	if (ratio)
		model = glm::scale(model, glm::vec2(ratio->val, 1.0f));
	return DisplayInfos::Matrix() * GetParentModelMatrix(R, e) * model;
}

glm::mat3 TNG::GetMatrixToDBSpace(entt::registry& R, entt::entity e) {
	glm::mat3 model = GetLocalTransform(R, e);
	return GetParentModelMatrixExcludingDB(R, e) * model;
}

glm::mat3 TNG::GetMatrixToTextureSpace(entt::registry& R, entt::entity e) {
	glm::mat3 mat = glm::inverse(GetMatrixToDBSpace(R, e));
	float DBratio = R.get<Cmp::AspectRatio>(GetDrawingBoard(R)).val;
	mat = glm::scale(mat, glm::vec2(2.0f * DBratio, 2.0f));
	mat = glm::translate(glm::mat3(1.0f), -glm::vec2(DBratio, 1.0f)) * mat;
	return mat;
}

glm::mat3 TNG::GetParentModelMatrix(entt::registry& R, entt::entity e) {
	Cmp::Parent* parent = R.try_get<Cmp::Parent>(e);
	if (parent)
		return GetParentModelMatrix(R, parent->id) * GetLocalTransform(R, parent->id);
	else
		return glm::mat3(1.0f);
}

glm::mat3 TNG::GetParentModelMatrixExcludingDB(entt::registry& R, entt::entity e) {
	Cmp::Parent* parent = R.try_get<Cmp::Parent>(e);
	if (parent && parent->id != GetDrawingBoard(R))
		return GetParentModelMatrixExcludingDB(R, parent->id) * GetLocalTransform(R, parent->id);
	else
		return glm::mat3(1.0f);
}