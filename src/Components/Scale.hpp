#pragma once

#include <glm/glm.hpp>

namespace Cmp {

struct Scale {
	Scale(float scale)
		: val(scale)
	{}

	float val;
};

}
