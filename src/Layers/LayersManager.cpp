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
	entt::entity id = createLayerEntity();

	RenderSystem& RS = I.renderSystem();
	Cmp::Texture& texture = I.registry().get<Cmp::Texture>(id);
	RS.setRenderTarget_Texture(texture);
		RenderSystem::s_shaderTest.bind();
		RenderSystem::s_shaderTest.setUniformMat3f("u_mat", glm::mat3(1.0f));
		glBindVertexArray(RenderSystem::m1to1QuadVAOid);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	RS.setRenderTarget_Screen();

	m_layersOrdered.push_back(id);
	return id;
}

entt::entity LayersManager::createPolygonLayer(const std::vector<glm::vec2>& vertices) {
	entt::entity id = createLayerEntity();
	I.registry().assign<entt::tag<"Polygon"_hs>>(id);
	Cmp::Vertices& cmpVertices = I.registry().assign<Cmp::Vertices>(id, vertices, id, I.shapeFactory());

	RenderSystem& RS = I.renderSystem();
	Cmp::Texture& texture = I.registry().get<Cmp::Texture>(id);
	RS.setRenderTarget_Texture(texture);
		RS.renderPolygon(cmpVertices.list, 32.0f);
	RS.setRenderTarget_Screen();

	m_layersOrdered.push_back(id);
	return id;
}

entt::entity LayersManager::createLayerEntity() {
	entt::registry& R = I.registry();
	entt::entity id = R.create();

	R.assign<Cmp::TransformMatrix>(id);
	R.assign<Cmp::Parent>(id, I.drawingBoardId());
	Cmp::Texture& texture = R.assign<Cmp::Texture>(id, I.renderSystem().previewWidth(), I.renderSystem().previewHeight());
	R.assign<Cmp::AspectRatio>(id, R.get<Cmp::AspectRatio>(I.drawingBoardId()));

	return id;
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
	glm::mat3 mat = I.getMatrixPlusAspectRatio(e);
	glm::vec2 posInModelSpace = glm::inverse(mat) * glm::vec3(posInNDC, 1.0f);
	Cmp::Texture* texture = I.registry().try_get<Cmp::Texture>(e);
	if (texture) {
		glm::vec2 posInNormalizedModelSpace = posInModelSpace * 0.5f + glm::vec2(0.5f);
		// get pixel color under mouse
		unsigned char pixelColor[4];
		I.renderSystem().setRenderTarget_Texture(*texture);
		glReadPixels(int(posInNormalizedModelSpace.x * texture->width), int(posInNormalizedModelSpace.y * texture->height), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor); // TODO this fails if preview texture is not in RGBA UNSIGNED BYTE
		I.renderSystem().setRenderTarget_Screen();
		return pixelColor[3] > 240;
	}
	else
		return (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f);
}