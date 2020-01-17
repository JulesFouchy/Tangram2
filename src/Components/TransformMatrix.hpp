#pragma once

#include <glm/glm.hpp>

namespace Cmp {
	struct TransformMatrix {
		glm::mat3 val;

		TransformMatrix()
			: val(1.0f)
		{}

	private:
		//Serialization
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(
				val[0][0], val[0][1], val[0][2],
				val[1][0], val[1][1], val[1][2], 
				val[2][0], val[2][1], val[2][2]
			);
		}
	};
}