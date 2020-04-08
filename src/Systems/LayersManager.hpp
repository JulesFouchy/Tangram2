#pragma once

#include "ISystem.hpp"

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
	entt::entity createFragmentLayer(const std::string& fragmentFilepath);
	entt::entity createPolygonLayer(const std::vector<glm::vec2>& vertices);

	entt::entity getEntityHoveredBy(const glm::vec2& posInNDC);
	entt::entity getEntityHoveredByMouse();

	void swapLayers(size_t i1, size_t i2);

	inline const std::vector<entt::entity>& getLayersOrdered() { return m_layersOrdered; }
	inline entt::entity getSelectedLayer() { return m_selectedLayer; }
	inline entt::entity& selectedLayer() { return m_selectedLayer; }

private:
	entt::entity createLayerEntity();
	entt::entity _createLayerBasedOnAShader(const std::string& vertexFilepath, const std::string& fragmentFilepath); // Beware : it doesn't give a tag to specify what kind of shader this is ; it must be done afterwards
	
	bool isEntityHoveredBy(entt::entity e, const glm::vec2& posInNDC);
friend class InputState_Rest;
	inline void setSelectedLayer(entt::entity e) { m_selectedLayer = e; }

private:
	unsigned int m_nbTestLayers = 0;
	std::unordered_map<std::string, unsigned int> m_nbFragmentLayersByName;

private:
	std::vector<entt::entity> m_layersOrdered;
	entt::entity m_selectedLayer;

	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(m_layersOrdered));
	}
};