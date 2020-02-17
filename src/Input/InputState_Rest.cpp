#include "InputState_Rest.hpp"
#include "InputState_Translate.hpp"
#include "InputState_GUI.hpp"
#include "Window_SaveAsProject.hpp"
#include "Window_ExportImage.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"
#include "Helper/File.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Components/Parent.hpp"

#include "GUI/FileBrowser.hpp"
#include "Debugging/Log.hpp"


InputState_Rest::InputState_Rest(Instance& instance)
	: IInputState(instance)
{}

entt::entity InputState_Rest::getFirstLayerOf(entt::entity e) {
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
	entt::entity hoveredEntity = I.layersManager().getEntityHoveredByMouse();
	// Get clicked layer
	I.layersManager().setSelectedLayer(getFirstLayerOf(hoveredEntity));
	// Translate 
	if (I.registry().valid(hoveredEntity))
		I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, hoveredEntity, MouseButton::Left);
}

void InputState_Rest::onWheelDown() {
	I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, I.drawingBoardId(), MouseButton::Wheel);
}

void InputState_Rest::onWheelScroll(float dl) {
	glm::mat3 mat = I.registry().get<Cmp::TransformMatrix>(I.drawingBoardId()).val();
	I.registry().replace<Cmp::TransformMatrix>(I.drawingBoardId(), glm::scale(mat, glm::vec2(pow(0.95f, -dl))));
}

void InputState_Rest::onKeyDown(SDL_Scancode key) {
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
	}
}