#pragma once

#include <glm/glm.hpp>

namespace MyMaths {
	glm::mat3 Scale(const glm::mat3& mat, const glm::vec2& scale, const glm::vec2& center);
}