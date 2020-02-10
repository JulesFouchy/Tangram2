#include "LayersManager.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"
#include "Components/Vertices.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Debugging/Log.hpp"

#include "OpenGL/Texture2D.hpp"
#include "Components/PreviewTexture.hpp"

#include "Render/RenderSystem.hpp"

LayersManager::LayersManager(Instance& instance)
	: ISystem(instance)
{}

entt::entity LayersManager::createTestLayer() {
	entt::registry& R = I.registry();
	entt::entity e = createLayerEntity();

	R.assign<entt::tag<"TestLayer"_hs>>(e);

	m_layersOrdered.push_back(e);
	return e;
}

entt::entity LayersManager::createPolygonLayer(const std::vector<glm::vec2>& vertices) {
	entt::registry& R = I.registry();
	entt::entity e = createLayerEntity();

	R.assign<entt::tag<"Polygon"_hs>>(e);
	R.assign<Cmp::Vertices>(e, vertices, e, I.shapeFactory());

	m_layersOrdered.push_back(e);
	return e;
}

entt::entity LayersManager::createLayerEntity() {
	entt::registry& R = I.registry();
	entt::entity e = R.create();

	R.assign<Cmp::TransformMatrix>(e);
	R.assign<Cmp::Parent>(e, I.drawingBoardId());
	R.assign<Cmp::Texture>(e, I.renderSystem().previewWidth(), I.renderSystem().previewHeight());

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