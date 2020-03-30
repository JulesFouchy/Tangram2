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

		Vertices() = default;

		Vertices(entt::registry& R, entt::entity parent, const std::vector<glm::vec2>& vertices) {
			for (const glm::vec2& vertex : vertices)
				list.push_back(ShapeFactory::CreatePoint2D(R, parent, vertex));
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