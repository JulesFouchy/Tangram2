#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Vertices.hpp"

#include "glm/gtx/matrix_transform_2d.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/String.hpp"
#include "Helper/File.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <imgui/imgui.h>

#include <cereal/archives/json.hpp>
#include <fstream>
#include "Debugging/Log.hpp"

#include "App.hpp"

Instance::Instance()
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this),
	  m_shapeFactory(*this),
	  m_projectLocation(MyFile::RootDir+"/MyTangramProjects"),
	  m_bUserChoseProjectName(false)
{
	// Project default name
	m_projectName = "UntitledProject0";
	int k = 1;
	while (MyFile::Exists(getProjectPath()) || App::Get().projectIsOpen(getProjectPath())) {
		m_projectName = "UntitledProject" + std::to_string(k++);
	}
	// Drawing board
	createDrawingBoard();
	//
	//m_shapeFactory.createPolygon({ glm::vec2(-0.3, -0.5), glm::vec2(0, 0), glm::vec2(0.8, -0.5), glm::vec2(-0.8, -0.5), glm::vec2(0.8, 0.5) });
	//
	{
		m_testLayer2 = layersManager().createTestLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(m_testLayer2).val;
		//mat = glm::translate(mat, glm::vec2(1.3f, 0.0f));
		mat = glm::scale(mat, glm::vec2(0.85f));
		//registry().get<Cmp::AspectRatio>(id).val = 2.0f;
	}
	
	{
		m_testLayer = layersManager().createTestLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(m_testLayer).val;
		//mat = glm::translate(mat, glm::vec2(1.0f, 0.0f));
		mat = glm::scale(mat, glm::vec2(0.5f));
		registry().get<Cmp::Parent>(m_testLayer).id = m_testLayer2;
	}

	m_poly = layersManager().createPolygonLayer({ glm::vec2(-0.3, -0.5), glm::vec2(0, 0), glm::vec2(0.8, -0.5), glm::vec2(-0.8, -0.5), glm::vec2(0.8, 0.5) });
}


Instance::Instance(const std::string& projectFolderpath)
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this),
	  m_shapeFactory(*this),
	  m_bUserChoseProjectName(true)
{
	openProject(projectFolderpath);
}

void Instance::onLoopIteration(){
	renderSystem().render();
	inputSystem().update();

	static float smoothMin = 32.0f;
	ImGui::Begin("Test");
	ImGui::SliderFloat("SmoothMin", &smoothMin, 0.0f, 256.0f);
	ImGui::End();

	layersManager().renderPolygonOnPreviewTexture(m_poly, smoothMin);


	renderSystem().setRenderTarget_Texture(registry().get<Cmp::Texture>(m_testLayer));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	RenderSystem::s_shaderTest.bind();
	RenderSystem::s_shaderTest.setUniformMat3f("u_localTransformMat", glm::scale(glm::inverse(getMatrixToDBSpace(m_testLayer)), glm::vec2(2.0f * registry().get<Cmp::AspectRatio>(drawingBoardId()).val, 2.0f)));
	glBindVertexArray(RenderSystem::m1to1QuadVAOid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	renderSystem().setRenderTarget_Screen();

	renderSystem().setRenderTarget_Texture(registry().get<Cmp::Texture>(m_testLayer2));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	RenderSystem::s_shaderTest.bind();
	RenderSystem::s_shaderTest.setUniformMat3f("u_localTransformMat", glm::scale(glm::inverse(getMatrixToDBSpace(m_testLayer2)), glm::vec2(2.0f * registry().get<Cmp::AspectRatio>(drawingBoardId()).val, 2.0f)));
	glBindVertexArray(RenderSystem::m1to1QuadVAOid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	renderSystem().setRenderTarget_Screen();
}

void Instance::createDrawingBoard() {
	m_drawingBoardId = registry().create();
	glm::mat3& mat = registry().assign<Cmp::TransformMatrix>(drawingBoardId()).val;
	mat = glm::scale(mat, glm::vec2(0.8f));
	mat = glm::rotate(mat, 0.1f);
	registry().assign<Cmp::AspectRatio>(drawingBoardId(), 16.0f / 9.0f);
}

glm::mat3 Instance::getMatrixPlusAspectRatio(entt::entity id) {
	glm::mat3 model = registry().get<Cmp::TransformMatrix>(id).val;
	Cmp::AspectRatio* ratio = registry().try_get<Cmp::AspectRatio>(id);
	if (ratio)
		model = glm::scale(model, glm::vec2(ratio->val, 1.0f));
	return DisplayInfos::Matrix() * getParentModelMatrix(id) * model;
}

glm::mat3 Instance::getMatrix(entt::entity id) {
	glm::mat3 model = getLocalTransform(id);
	return DisplayInfos::Matrix() * getParentModelMatrix(id) * model;
}

glm::mat3 Instance::getMatrixToDBSpace(entt::entity id) {
	glm::mat3 model = getLocalTransform(id);
	return getParentModelMatrixExcludingDB(id) * model;
}

glm::mat3 Instance::getLocalTransform(entt::entity id) {
	return registry().get<Cmp::TransformMatrix>(id).val;
}

glm::mat3 Instance::getParentModelMatrix(entt::entity id) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(id);
	if (parent)
		return getParentModelMatrix(parent->id) * registry().get<Cmp::TransformMatrix>(parent->id).val;
	else
		return glm::mat3(1.0f);
}

glm::mat3 Instance::getParentModelMatrixExcludingDB(entt::entity id) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(id);
	if (parent && parent->id != drawingBoardId())
		return getParentModelMatrixExcludingDB(parent->id) * registry().get<Cmp::TransformMatrix>(parent->id).val;
	else
		return glm::mat3(1.0f);
}

std::string Instance::getProjectPath() {
	return m_projectLocation + "/" + (m_projectName.empty() ? "UntitledProject" : m_projectName);
}

void Instance::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		if (!ImGui::GetIO().WantCaptureMouse) {
			inputSystem().onWheelScroll((float)e.wheel.y);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				inputSystem().onLeftClicDown();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				inputSystem().onRightClicDown();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				inputSystem().onLeftClicUp();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				inputSystem().onRightClicUp();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;


	case SDL_KEYDOWN:
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			inputSystem().onKeyDown(e.key.keysym.scancode);
		}
		break;

	case SDL_KEYUP:
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			inputSystem().onKeyUp(e.key.keysym.scancode);
		}
		break;

	default:
		break;
	}
}

void Instance::saveProject(const std::string& folderpath) {
	m_projectLocation = MyString::GetFolderHierarchy(folderpath);
	m_projectName = MyString::RemoveFolderHierarchy(folderpath);

	spdlog::info("Saving project to '{}'", folderpath);
	std::ofstream registryOs(folderpath+"/reg.tng");
	std::ofstream otherOs(folderpath + "/other.tng");
	{
		cereal::JSONOutputArchive otherArchive(otherOs);
		otherArchive(
			CEREAL_NVP(m_layersManager),
			CEREAL_NVP(m_drawingBoardId)
		);
		cereal::JSONOutputArchive registryArchive(registryOs);
		registry().snapshot()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component<Cmp::AspectRatio, Cmp::TransformMatrix, Cmp::Parent, Cmp::Vertices,
						entt::tag<"Point2D"_hs>, entt::tag<"Polygon"_hs>>(registryArchive);
	}
	Log::separationLine();
}

void Instance::openProject(const std::string& folderpath) {
	m_projectLocation = MyString::GetFolderHierarchy(folderpath);
	m_projectName = MyString::RemoveFolderHierarchy(folderpath);

	spdlog::info("Opening project from '{}'", folderpath);
	std::ifstream registryIs(folderpath + "/reg.tng");
	std::ifstream otherIs(folderpath + "/other.tng");
	{
		cereal::JSONInputArchive otherArchive(otherIs);
		otherArchive(
			m_layersManager,
			m_drawingBoardId
		);
		cereal::JSONInputArchive registryArchive(registryIs);
		registry().loader()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component<Cmp::AspectRatio, Cmp::TransformMatrix, Cmp::Parent, Cmp::Vertices,
				entt::tag<"Point2D"_hs>, entt::tag<"Polygon"_hs>>(registryArchive);
	}
	Log::separationLine();
}