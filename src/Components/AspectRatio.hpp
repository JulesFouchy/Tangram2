#pragma once

namespace Cmp {
	struct AspectRatio {
		float val;

	private:
		//Serialization
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(val));
		}
	};
}