#pragma once

#include <entt/entt.hpp>

#include "Shapes/ShapeFactory.hpp"

#include <glm/glm.hpp>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

namespace Cmp {
	struct Vertices {
		std::vector<entt::entity> list;

		Vertices(const std::vector<glm::vec2>& vertices, const ShapeFactory& shapeFactory) {
			for (const glm::vec2& vertex : vertices)
				list.push_back(shapeFactory.createPoint2D(vertex));
		}

	private:
		//Serialization
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(list));
		}
	};
}