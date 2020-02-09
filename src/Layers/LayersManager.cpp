#include "LayersManager.hpp"

#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Parent.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Debugging/Log.hpp"

#include "OpenGL/Texture2D.hpp"
#include "Components/PreviewTexture.hpp"

#include "Render/RenderSystem.hpp"

LayersManager::LayersManager(Instance& instance)
	: ISystem(instance)
{}

entt::entity LayersManager::addLayer() {
	entt::entity id = createLayerEntity();
	m_layersOrdered.push_back(id);
	return id;
}

entt::entity LayersManager::createLayerEntity() {
	entt::registry& R = I.registry();
	RenderSystem& RS = I.renderSystem();
	entt::entity id = R.create();

	R.assign<Cmp::TransformMatrix>(id);
	R.assign<Cmp::Parent>(id, I.drawingBoardId());
	Cmp::Texture& texture = R.assign<Cmp::Texture>(id, I.renderSystem().previewWidth(), I.renderSystem().previewHeight());
	RS.setRenderTarget_Texture(texture);
		RenderSystem::s_shaderTest.bind();
		RenderSystem::s_shaderTest.setUniformMat3f("u_mat", glm::mat3(1.0f));
		glBindVertexArray(RenderSystem::m1to1QuadVAOid);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	RS.setRenderTarget_Screen();
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
	return (abs(posInModelSpace.x) < 1.0f && abs(posInModelSpace.y) < 1.0f);
}