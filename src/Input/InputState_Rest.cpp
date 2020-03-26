#include "InputState_Rest.hpp"
#include "InputState_Translate.hpp"
#include "InputState_GUI.hpp"
#include "Window_SaveAsProject.hpp"
#include "Window_ExportImage.hpp"
#include "Window_CreateFragmentLayer.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"
#include "Helper/File.hpp"

#include "Helper/Geometry.hpp"
#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Components/Parent.hpp"

#include "Systems/HistoryManager.hpp"

#include "GUI/FileBrowser.hpp"
#include "Debugging/Log.hpp"


InputState_Rest::InputState_Rest(Instance& instance)
	: IInputState(instance)
{}

entt::entity InputState_Rest::getFirstLayerOf(entt::entity e) {
	// (e.g. if you click on the handle of a polygon, this will return the associated polygon layer)
	entt::registry& R = I.registry();
	if (R.valid(e)) {
		if (R.has<entt::tag<"Layer"_hs>>(e))
			return e;
		else {
			Cmp::Parent* parent = R.try_get<Cmp::Parent>(e);
			if (parent)
				return getFirstLayerOf(parent->id);
			else
				return entt::null;
		}
	}
	else
		return entt::null;
}

void InputState_Rest::onLeftClicDown() {
	entt::entity clickedEntity = I.layersManager().getEntityHoveredByMouse();
	entt::entity clickedLayer = getFirstLayerOf(clickedEntity);
	I.layersManager().setSelectedLayer(clickedLayer);
	// Translate 
	if (I.registry().valid(clickedEntity))
		I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, clickedEntity, MouseButton::Left);
}

void InputState_Rest::onWheelDown() {
	I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, I.drawingBoardId(), MouseButton::Wheel);
}

void InputState_Rest::onWheelScroll(float dl) {
	glm::mat3 mat = I.registry().get<Cmp::TransformMatrix>(I.drawingBoardId()).val();
	glm::vec2 transformCenter = glm::inverse(mat) * glm::vec3(DisplayInfos::MousePositionInScreen(), 1.0f);
	I.registry().replace<Cmp::TransformMatrix>(I.drawingBoardId(), MyMaths::Scale(mat, glm::vec2(pow(0.95f, -dl)), transformCenter));
}

void InputState_Rest::onKeyDown(SDL_Scancode key) {
	// CTRL
	if (DisplayInfos::KeyIsDown(SDL_SCANCODE_LCTRL)) {
		if (key == SDL_SCANCODE_E) {
			I.inputSystem().setGUIState<Window_ExportImage>();
		}
		else if (key == SDL_SCANCODE_I) {
			std::string filepath = FileBrowser::GetFileOpen(FileFilter::Image);
			spdlog::info("Importing file '{}'", filepath);
		}
		else if (key == SDL_SCANCODE_S) {
			if (!MyFile::Exists(I.getProjectPath()) || !I.m_bUserChoseProjectName || DisplayInfos::KeyIsDown(SDL_SCANCODE_LSHIFT))
				I.inputSystem().setGUIState<Window_SaveAsProject>();
			else
				I.saveProject(I.getProjectPath());
		}
		else if (key == SDL_SCANCODE_F) {
			I.inputSystem().setGUIState<Window_CreateFragmentLayer>();
		}
		else if (key == SDL_SCANCODE_W) {
			HistoryManager::MoveBackward(I.registry(), I.layersManager().selectedLayer());
		}
		else if (key == SDL_SCANCODE_Y) {
			HistoryManager::MoveForward(I.registry(), I.layersManager().selectedLayer());
		}
	}
	// no modifier key
	else {
		if (key == SDL_SCANCODE_TAB) {
			GUISystem::ToggleOnOff();
		}
	}
}