#pragma once

#include <glm/glm.hpp>

namespace Cmp {
	struct TransformMatrix {
		glm::mat3 val;

		TransformMatrix()
			: val(1.0f)
		{}
	};
}