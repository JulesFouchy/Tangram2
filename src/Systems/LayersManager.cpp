#include "LayersManager.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/Vertices.hpp"
#include "Components/Parameters.hpp"
#include "Components/PreviewTexture.hpp"
#include "Components/Name.hpp"
#include "Components/ShaderReference.hpp"
#include "Components/History.hpp"

#include "ShaderSystem.hpp"
#include "RenderSystem.hpp"

#include "Core/CreateParentRelationship.hpp"
#include "Core/GetDrawingBoard.hpp"
#include "Core/GetMatrix.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/String.hpp"

#include "Settings/Settings.hpp"

LayersManager::LayersManager()
	: m_selectedLayer(entt::null)
{}

entt::entity LayersManager::createLayerEntity(entt::registry& R) {
	entt::entity e = R.create();
	m_layersOrdered.push_back(e);

	// Layer tag
	R.assign<entt::tag<"Layer"_hs>>(e);
	// Transform Matrix
	R.assign<Cmp::TransformMatrix>(e);
	// Parent / Children
	R.assign<Cmp::Parent>(e, entt::null);
	TNG::CreateParentRelationship(R, e, TNG::GetDrawingBoard(R));
	R.assign<Cmp::Children>(e);
	// Texture
	R.assign<Cmp::Texture>(e, Settings::GetPREVIEW_SIZE_IN_PX(), Settings::GetPREVIEW_SIZE_IN_PX());
	R.assign<entt::tag<"MustRecomputeTexture"_hs>>(e);
	// Parameters
	R.assign<Cmp::Parameters>(e);
	// History
	R.assign<Cmp::History>(e);

	return e;
}

entt::entity LayersManager::_createLayerBasedOnAShader(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	entt::entity e = createLayerEntity(R);

	// Name
	std::string shaderName = MyString::RemoveFolderHierarchy(MyString::RemoveFileExtension(fragmentFilepath));
	if (m_nbFragmentLayersByName.find(shaderName) == m_nbFragmentLayersByName.end())
		m_nbFragmentLayersByName[shaderName] = 0;
	R.assign<Cmp::Name>(e, shaderName + "_" + std::to_string(m_nbFragmentLayersByName[shaderName]));
	m_nbFragmentLayersByName[shaderName]++;
	// Shader
	entt::entity shader = ShaderSystem::Create(R, vertexFilepath, fragmentFilepath);
	R.assign<Cmp::ShaderReference>(e, shader);
	ShaderSystem::CompileShaderAndUpdateParametersList(R, e);
	return e;
}

entt::entity LayersManager::createTestLayer(entt::registry& R) {
	entt::entity e = createLayerEntity(R);

	R.assign<entt::tag<"TestLayer"_hs>>(e);
	R.assign<Cmp::Name>(e, "Test" + std::to_string(m_nbTestLayers));
	m_nbTestLayers++;

	return e;
}

entt::entity LayersManager::createFragmentLayer(entt::registry& R, const std::string& fragmentFilepath) {
	entt::entity e = _createLayerBasedOnAShader(R, "res/shaders/defaultDrawOnTexture.vert", fragmentFilepath);
	R.assign<entt::tag<"FragmentLayer"_hs>>(e);
	return e;
}

entt::entity LayersManager::createPolygonLayer(entt::registry& R, const std::vector<glm::vec2>& vertices) {
	entt::entity e = createFragmentLayer(R, "res/shaders/polygon.frag");

	R.assign<entt::tag<"Polygon"_hs>>(e);
	//R.assign<Cmp::Vertices>(e, R, e, vertices);



	//Cmp::Parameters& params = R.get<Cmp::Parameters>(e);
	//R.assign<Cmp::SliderFloat>(e2, "Smooth Min", 0.036f, 0.0f, 0.1f);
	//params.list.push_back(std::make_unique<FloatParameter>();

	return e;
}

entt::entity LayersManager::getEntityHoveredBy(entt::registry& R, RenderSystem& renderSystem, const glm::vec2& posInNDC) {
	// Check Points2D	
	auto points2D = R.view<entt::tag<"Point2D"_hs>>();
	for (auto entity : points2D) {
		if (isEntityHoveredBy(R, renderSystem, entity, posInNDC))
			return entity;
	}
	// Check layers
	for (auto it = m_layersOrdered.crbegin(); it < m_layersOrdered.crend(); it++) {
		if (isEntityHoveredBy(R, renderSystem, *it, posInNDC))
			return *it;
	}
	return entt::null;
}

entt::entity LayersManager::getEntityHoveredByMouse(entt::registry& R, RenderSystem& renderSystem) {
	return getEntityHoveredBy(R, renderSystem, DisplayInfos::MousePositionInNormalizedDeviceCoordinates());
}

bool LayersManager::isEntityHoveredBy(entt::registry& R, RenderSystem& renderSystem, entt::entity e, const glm::vec2& posInNDC) {
	Cmp::Texture* texture = R.try_get<Cmp::Texture>(e);
	if (texture) {
		glm::mat3 mat = TNG::GetMatrixPlusAspectRatio(R, TNG::GetDrawingBoard(R));
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		if ((abs(posInModelSpace.x) > 1.0f || abs(posInModelSpace.y) > 1.0f)) // outside of drawingBoard
			return false;
		glm::vec2 posInNormalizedModelSpace = posInModelSpace * 0.5f + glm::vec2(0.5f);
		// get pixel color
		unsigned char pixelColor[4];
		renderSystem.setRenderTarget_Texture(*texture);
		glReadPixels(int(posInNormalizedModelSpace.x * texture->width), int(posInNormalizedModelSpace.y * texture->height), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor); // TODO this fails if preview texture is not in RGBA UNSIGNED BYTE
		renderSystem.setRenderTarget_Screen();
		return pixelColor[3] > Settings::GetMIN_OPACITY_TO_GRAB_LAYER();
	}
	else {
		glm::mat3 mat = TNG::GetMatrixPlusAspectRatio(R, e);
		glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
		return (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f);
	}
}

void LayersManager::swapLayers(size_t i1, size_t i2) {
	entt::entity tmp = m_layersOrdered[i1];
	m_layersOrdered[i1] = m_layersOrdered[i2];
	m_layersOrdered[i2] = tmp;
}