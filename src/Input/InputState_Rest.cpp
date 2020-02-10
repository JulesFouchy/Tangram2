#include "InputState_Rest.hpp"
#include "InputState_Translate.hpp"
#include "InputState_GUI.hpp"
#include "Window_SaveAsProject.hpp"

#include "Instance.hpp"
#include "Helper/DisplayInfos.hpp"
#include "Helper/File.hpp"

#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "GUI/FileBrowser.hpp"
#include "Debugging/Log.hpp"


InputState_Rest::InputState_Rest(Instance& instance)
	: IInputState(instance)
{}

void InputState_Rest::onLeftClicDown() {
	if (DisplayInfos::KeyIsDown(SDL_SCANCODE_SPACE))
		I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, I.drawingBoardId());
	else {
		entt::entity hoveredEntity = I.layersManager().getEntityHoveredByMouse();
		if (I.registry().valid(hoveredEntity))
			I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, hoveredEntity);
		else {
			// Create point2D
			//glm::vec2 pos = DisplayInfos::MousePositionInNormalizedDeviceCoordinates();
			//glm::vec2 posInDBSpace = glm::inverse(I.getMatrix(I.drawingBoardId())) * glm::vec3(pos, 1.0f);
			//I.shapeFactory().createPoint2D(posInDBSpace);
		}
	}
}

void InputState_Rest::onWheelScroll(float dl) {
	glm::mat3 mat = I.registry().get<Cmp::TransformMatrix>(I.drawingBoardId()).val();
	I.registry().replace<Cmp::TransformMatrix>(I.drawingBoardId(), glm::scale(mat, glm::vec2(pow(0.95f, -dl))));
}

void InputState_Rest::onKeyDown(SDL_Scancode key) {
	if (DisplayInfos::KeyIsDown(SDL_SCANCODE_LCTRL)) {
		if (key == SDL_SCANCODE_E) {
			std::string filepath = FileBrowser::GetFileSave(FileFilter::Image);
			spdlog::info("Saving file '{}'", filepath);
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