#pragma once

#include <glm/glm.hpp>

namespace Cmp {
	struct PreviewTexture {
		unsigned int width;
		unsigned int height;
		unsigned int id;

		PreviewTexture()
			: id(0)
		{}
}