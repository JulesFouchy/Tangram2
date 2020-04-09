#include "InputState_Rest.hpp"

#include "InputState_Translate.hpp"
#include "InputState_GUI.hpp"

#include "Window_SaveAsProject.hpp"
#include "Window_ExportImage.hpp"
#include "Window_CreateFragmentLayer.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/File.hpp"

#include "Helper/Geometry.hpp"
#include "Components/TransformMatrix.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Components/Parent.hpp"

#include "Core/ChangeActiveHistory.hpp"
#include "Core/GetDrawingBoard.hpp"

#include "Components/ShaderReference.hpp"

#include "InputSystem.hpp"
#include "Systems/ShaderSystem.hpp"
#include "Systems/GUISystem.hpp"
#include "Systems/HistoryManager.hpp"

#include "GUI/FileBrowser.hpp"
#include "Debugging/Log.hpp"

InputState_Rest::InputState_Rest(InputSystem& inputSystem)
	: IInputState(inputSystem)
{}

void InputState_Rest::onLeftClicDown(entt::registry& R, entt::entity clickedEntity, entt::entity clickedLayer, entt::entity& rSelectedLayer) {
	rSelectedLayer = clickedLayer;
	// Translate 
	if (R.valid(clickedEntity)) {
		if (clickedEntity == clickedLayer)
			TNG::SetActiveHistoryToTransform(R, clickedLayer);
		else // because Point2D handles are stored in ParametersHistory
			TNG::SetActiveHistoryToParameters(R, clickedLayer);
		m_inputSystem.m_currentState = std::make_unique<InputState_Translate>(clickedEntity, MouseButton::Left);
	}
}

void InputState_Rest::onWheelDown(entt::registry& R) {
	I.inputSystem().m_currentState = std::make_unique<InputState_Translate>(TNG::GetDrawingBoard(R), MouseButton::Wheel);
}

void InputState_Rest::onWheelScroll(entt::registry& R, float dl) {
	entt::entity drawingBoard = TNG::GetDrawingBoard(I.registry());
	glm::mat3 mat = R.get<Cmp::TransformMatrix>(drawingBoard).val();
	glm::vec2 transformCenter = glm::inverse(mat) * glm::vec3(DisplayInfos::MousePositionInScreen(), 1.0f);
	I.registry().replace<Cmp::TransformMatrix>(drawingBoard, MyMaths::Scale(mat, glm::vec2(pow(0.95f, -dl)), transformCenter));
	HistoryManager::RecordTransform(I.registry(), drawingBoard, mat);
}

void InputState_Rest::onKeyDown(entt::registry& R, SDL_Scancode key) {
	// CTRL
	if (DisplayInfos::KeyIsDown(SDL_SCANCODE_LCTRL)) {
		if (key == SDL_SCANCODE_E) {
			m_inputSystem.setGUIState<Window_ExportImage>();
		}
		else if (key == SDL_SCANCODE_I) {
			std::string filepath = FileBrowser::GetFileOpen(FileFilter::Image);
			spdlog::info("Importing file '{}'", filepath);
		}
		else if (key == SDL_SCANCODE_S) {
			if (!MyFile::Exists(I.getProjectPath()) || !I.m_bUserChoseProjectName || DisplayInfos::KeyIsDown(SDL_SCANCODE_LSHIFT))
				m_inputSystem.setGUIState<Window_SaveAsProject>();
			else
				I.saveProject(I.getProjectPath());
		}
		else if (key == SDL_SCANCODE_F) {
			m_inputSystem.setGUIState<Window_CreateFragmentLayer>();
		}
		else if (key == SDL_SCANCODE_W) {
			entt::entity layer = I.layersManager().selectedLayer();
			if (R.valid(layer))
				HistoryManager::MoveBackward(I.registry(), layer);
			else
				HistoryManager::MoveBackward(I.registry(), TNG::GetDrawingBoard(I.registry()));
		}
		else if (key == SDL_SCANCODE_Y) {
			entt::entity layer = I.layersManager().selectedLayer();
			if (I.registry().valid(layer))
				HistoryManager::MoveForward(I.registry(), layer);
			else
				HistoryManager::MoveForward(I.registry(), TNG::GetDrawingBoard(I.registry()));
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