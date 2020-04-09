#pragma once

namespace Cmp {
struct TransformMatrix {
	TransformMatrix()
		: m_val(1.0f)
	{}
	TransformMatrix(const glm::mat3& mat)
		: m_val(mat)
	{}

	inline const glm::mat3& val() { return m_val; }

private:
	glm::mat3 m_val;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			m_val[0][0], m_val[0][1], m_val[0][2],
			m_val[1][0], m_val[1][1], m_val[1][2], 
			m_val[2][0], m_val[2][1], m_val[2][2]
		);
	}
};
}