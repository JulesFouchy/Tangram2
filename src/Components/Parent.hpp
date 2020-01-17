#pragma once

#include <entt/entt.hpp>
#include <cereal/access.hpp>

namespace Cmp {
	struct Parent {
		entt::entity id;

	private:
		//Serialization
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(id));
		}
	};
}