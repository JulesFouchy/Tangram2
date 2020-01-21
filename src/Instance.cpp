#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"

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
	  m_projectLocation(MyFile::RootDir),
	  m_bUserChoseProjectName(false)
{
	// Project default name
	m_projectName = "UntitledProject0";
	int k = 1;
	while (MyFile::Exists(getProjectPath()) || App::Get().projectIsOpen(getProjectPath())) {
		m_projectName = "UntitledProject" + std::to_string(k++);
	}
	//
	createDrawingBoard();
	
	entt::entity id1;
	{
		entt::entity id = layersManager().addLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(id).val;
		mat = glm::translate(mat, glm::vec2(1.3f, 0.0f));
		mat = glm::scale(mat, glm::vec2(0.15f));
		registry().get<Cmp::AspectRatio>(id).val = 2.0f;
		id1 = id;
	}
	
	{
		entt::entity id = layersManager().addLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(id).val;
		mat = glm::translate(mat, glm::vec2(1.0f, 0.0f));
		//mat = glm::scale(mat, glm::vec2(0.3f));
		registry().get<Cmp::Parent>(id).id = id1;
	}
}


Instance::Instance(const std::string& projectFolderpath)
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this),
	  m_bUserChoseProjectName(true)
{
	openProject(projectFolderpath);
}

void Instance::onLoopIteration(){
	glClear(GL_COLOR_BUFFER_BIT);
	renderSystem().render();
	inputSystem().update();
}

void Instance::createDrawingBoard() {
	m_drawingBoardId = registry().create();
	glm::mat3& mat = registry().assign<Cmp::TransformMatrix>(drawingBoardId()).val;
	mat = glm::scale(mat, glm::vec2(0.8f));
	mat = glm::rotate(mat, 0.1f);
	registry().assign<Cmp::AspectRatio>(drawingBoardId(), 16.0f / 9.0f);
}

glm::mat3 Instance::getMatrix(entt::entity id) {
	glm::mat3 model = registry().get<Cmp::TransformMatrix>(id).val;
	model = glm::scale(model, glm::vec2(registry().get<Cmp::AspectRatio>(id).val, 1.0f));
	return DisplayInfos::Matrix() * getParentModelMatrix(id) * model;
}

glm::mat3 Instance::getParentModelMatrix(entt::entity id) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(id);
	if (parent)
		return getParentModelMatrix(parent->id) * registry().get<Cmp::TransformMatrix>(parent->id).val;
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
			.component<Cmp::AspectRatio, Cmp::TransformMatrix, Cmp::Parent>(registryArchive);
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
			.component<Cmp::AspectRatio, Cmp::TransformMatrix, Cmp::Parent>(registryArchive);
	}
	Log::separationLine();
}