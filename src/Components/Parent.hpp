#pragma once

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