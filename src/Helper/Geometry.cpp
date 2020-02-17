#include "Geometry.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>

glm::mat3 MyMaths::Scale(const glm::mat3& mat, const glm::vec2& scale, const glm::vec2& center) {
	glm::mat3 res(mat);
	res = glm::translate(res, center);
	res = glm::scale(res, scale);
	return glm::translate(res, -center);
}