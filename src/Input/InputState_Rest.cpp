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

#include "Core/ChangeActiveHistory.hpp"

#include "Components/ShaderReference.hpp"

#include "Systems/ShaderSystem.hpp"
#include "Systems/GUISystem.hpp"
#include "Systems/HistoryManager.hpp"

#include "GUI/FileBrowser.hpp"
#include "Debugging/Log.hpp"


InputState_Rest::InputState_Rest(Instance& instance)
	: IInputState(instance)
{}

void InputState_Rest::onLeftClicDown(entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) {
	rSelectedLayer = clickedLayer;
	// Translate 
	if (I.registry().valid(clickedEntity)) {
		if (clickedEntity == clickedLayer)
			TNG::SetActiveHistoryToTransform(I.registry(), clickedLayer);
		else // because Point2D handles are stored in ParametersHistory
			TNG::SetActiveHistoryToParameters(I.registry(), clickedLayer);
		I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, clickedEntity, MouseButton::Left);
	}
}

void InputState_Rest::onWheelDown() {
	I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(I, I.drawingBoardId(), MouseButton::Wheel);
}

void InputState_Rest::onWheelScroll(float dl) {
	glm::mat3 mat = I.registry().get<Cmp::TransformMatrix>(I.drawingBoardId()).val();
	glm::vec2 transformCenter = glm::inverse(mat) * glm::vec3(DisplayInfos::MousePositionInScreen(), 1.0f);
	I.registry().replace<Cmp::TransformMatrix>(I.drawingBoardId(), MyMaths::Scale(mat, glm::vec2(pow(0.95f, -dl)), transformCenter));
	HistoryManager::RecordTransform(I.registry(), I.drawingBoardId(), mat);
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
			entt::entity layer = I.layersManager().selectedLayer();
			if (I.registry().valid(layer))
				HistoryManager::MoveBackward(I.registry(), layer);
			else
				HistoryManager::MoveBackward(I.registry(), I.drawingBoardId());
		}
		else if (key == SDL_SCANCODE_Y) {
			entt::entity layer = I.layersManager().selectedLayer();
			if (I.registry().valid(layer))
				HistoryManager::MoveForward(I.registry(), layer);
			else
				HistoryManager::MoveForward(I.registry(), I.drawingBoardId());
		}
	}
	// no modifier key
	else {
		if (key == SDL_SCANCODE_TAB) {
			GUISystem::ToggleOnOff();
		}
		else if (key == SDL_SCANCODE_F5) {
			ShaderSystem::TryReloadShader(I.registry(), I.layersManager().selectedLayer());
		}
	}
}