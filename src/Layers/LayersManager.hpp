#pragma once

#include "System/ISystem.hpp"

#include <entt/entt.hpp>

#include <vector>

#include <glm/glm.hpp>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

class LayersManager : public ISystem {
friend class RenderSystem;
public:
	LayersManager(Instance& instance);
	~LayersManager() = default;

	entt::entity createTestLayer();
	entt::entity createPolygonLayer(const std::vector<glm::vec2>& vertices);
	void renderPolygonOnPreviewTexture(entt::entity polygon, float smoothMin);

	entt::entity getEntityHoveredBy(const glm::vec2& posInNDC);
	entt::entity getEntityHoveredByMouse();

private:
	entt::entity createLayerEntity();
	bool isEntityHoveredBy(entt::entity e, const glm::vec2& posInNDC);

private:
	std::vector<entt::entity> m_layersOrdered;


	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(m_layersOrdered));
	}
};