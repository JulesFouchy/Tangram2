#pragma once

#include <glm.hpp>

namespace Cmp {

struct Translation {
	Translation(float x, float y)
		: val(x, y)
	{}

	operator glm::vec2() {
		return val;
	}

	glm::vec2 val;
};

}
