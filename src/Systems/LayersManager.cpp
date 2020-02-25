#include "LayersManager.hpp"

#include "Instance.hpp"

#include "RenderSystem.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/Vertices.hpp"
#include "Components/ParametersList.hpp"
#include "Components/GUI/SliderFloat.hpp"
#include "Components/PreviewTexture.hpp"
#include "Components/Name.hpp"
#include "Components/Shader.hpp"
#include "Components/ShaderReference.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/String.hpp"

#include "Debugging/Log.hpp"

LayersManager::LayersManager(Instance& instance)
	: ISystem(instance), 
	  m_selectedLayer(entt::null)
{}

entt::entity LayersManager::createTestLayer() {
	entt::registry& R = I.registry();
	entt::entity e = createLayerEntity();

	R.assign<entt::tag<"TestLayer"_hs>>(e);
	R.assign<Cmp::Name>(e, "Test" + std::to_string(m_nbTestLayers));
	m_nbTestLayers++;

	m_layersOrdered.push_back(e);
	return e;
}

entt::entity LayersManager::createFragmentLayer(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	entt::registry& R = I.registry();
	entt::entity e = createLayerEntity();

	R.assign<entt::tag<"FragmentLayer"_hs>>(e);
	std::string shaderName = MyString::RemoveFolderHierarchy(MyString::RemoveFileExtension(fragmentFilepath));
	if (m_nbFragmentLayersByName.find(shaderName) == m_nbFragmentLayersByName.end())
		m_nbFragmentLayersByName[shaderName] = 0;
	R.assign<Cmp::Name>(e, shaderName + std::to_string(m_nbFragmentLayersByName[shaderName]));
	m_nbFragmentLayersByName[shaderName]++;

	entt::entity shader = instantiateShader(vertexFilepath, fragmentFilepath, R);
	R.assign<Cmp::ShaderReference>(e, shader);

	m_layersOrdered.push_back(e);
	return e;
}

entt::entity LayersManager::createPolygonLayer(const std::vector<glm::vec2>& vertices) {
	entt::registry& R = I.registry();
	entt::entity e = createLayerEntity();

	R.assign<entt::tag<"Polygon"_hs>>(e);
	R.assign<Cmp::Vertices>(e, vertices, e, I.shapeFactory());
	R.assign<Cmp::Name>(e, "Polygon" + std::to_string(m_nbPolygonLayers));
	m_nbPolygonLayers++;

	Cmp::Parameters& params = R.get<Cmp::Parameters>(e);
	entt::entity e2 = R.create();
	R.assign<Cmp::SliderFloat>(e2, "Smooth Min", 0.036f, 0.0f, 0.1f);
	params.list.push_back(e2);

	m_layersOrdered.push_back(e);
	return e;
}

entt::entity LayersManager::createLayerEntity() {
	entt::registry& R = I.registry();
	entt::entity e = R.create();

	R.assign<entt::tag<"Layer"_hs>>(e);
	R.assign<Cmp::TransformMatrix>(e);
	R.assign<Cmp::Parent>(e, entt::null);
	I.setParentOf(e, I.drawingBoardId());
	R.assign<Cmp::Children>(e);
	R.assign<Cmp::Texture>(e, I.renderSystem().previewWidth(), I.renderSystem().previewHeight());
	R.assign<entt::tag<"MustRecomputeTexture"_hs>>(e);
	R.assign<Cmp::Parameters>(e);
	return e;
}

entt::entity LayersManager::instantiateShader(const std::string& vertexFilepath, const std::string& fragmentFilepath, entt::registry& R) const {
	entt::entity e = R.create();
	R.assign<Cmp::Shader>(e, vertexFilepath, fragmentFilepath);
	return e;
}

entt::entity LayersManager::getEntityHoveredBy(const glm::vec2& posInNDC) {
	// Check Points2D	
	auto points2D = I.registry().view<entt::tag<"Point2D"_hs>>();
	for (auto entity : points2D) {
		if (isEntityHoveredBy(entity, posInNDC))
			return entity;
	}
	// Check layers
	for (auto it = m_layersOrdered.crbegin(); it < m_layersOrdered.crend(); it++) {
		if (isEntityHoveredBy(*it, posInNDC))
			return *it;
	}
	return entt::null;
}

entt::entity LayersManager::getEntityHoveredByMouse() {
	return getEntityHoveredBy(DisplayInfos::MousePositionInNormalizedDeviceCoordinates());
}

bool LayersManager::isEntityHoveredBy(entt::entity e, const glm::vec2& posInNDC) {
	Cmp::Texture* texture = I.registry().try_get<Cmp::Texture>(e);
	if (texture) {
		glm::mat3 mat = I.getMatrixPlusAspectRatio(I.drawingBoardId());
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		if ((abs(posInModelSpace.x) > 1.0f || abs(posInModelSpace.y) > 1.0f)) // outside of drawingBoard
			return false;
		glm::vec2 posInNormalizedModelSpace = posInModelSpace * 0.5f + glm::vec2(0.5f);
		// get pixel color
		unsigned char pixelColor[4];
		I.renderSystem().setRenderTarget_Texture(*texture);
		glReadPixels(int(posInNormalizedModelSpace.x * texture->width), int(posInNormalizedModelSpace.y * texture->height), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor); // TODO this fails if preview texture is not in RGBA UNSIGNED BYTE
		I.renderSystem().setRenderTarget_Screen();
		return pixelColor[3] > 240;
	}
	else {
		glm::mat3 mat = I.getMatrixPlusAspectRatio(e);
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		return (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f);
	}
}

void LayersManager::swapLayers(size_t i1, size_t i2) {
	entt::entity tmp = m_layersOrdered[i1];
	m_layersOrdered[i1] = m_layersOrdered[i2];
	m_layersOrdered[i2] = tmp;
}