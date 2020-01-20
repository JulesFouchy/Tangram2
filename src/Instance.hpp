#pragma once

union SDL_Event;

#include <entt/entt.hpp>
#include <string>

#include "Render/RenderSystem.hpp"
#include "Input/InputSystem.hpp"
#include "Layers/LayersManager.hpp"

class Instance {
public:
	Instance();
	Instance(const std::string& projectFolderpath);
	~Instance() = default;

	glm::mat3 getMatrix(entt::entity id);
	glm::mat3 getParentModelMatrix(entt::entity id);

	std::string getProjectPath();

	inline entt::registry& registry() { return m_registry; }
	inline RenderSystem& renderSystem() { return m_renderSystem; }
	inline InputSystem& inputSystem() { return m_inputSystem; }
	inline LayersManager& layersManager() { return m_layersManager; }
	inline entt::entity& drawingBoardId() { return m_drawingBoardId; }

private:
friend class App;
	void onLoopIteration();
	void onEvent(const SDL_Event& e);
	void createDrawingBoard();
	void onWindowResize() {};
friend class Window_SaveAsProject;
friend class InputState_Rest;
	void saveProject(const std::string& folderpath);
	void openProject(const std::string& folderpath);

private:
	entt::registry m_registry;
	RenderSystem m_renderSystem;
	InputSystem m_inputSystem;
	LayersManager m_layersManager;
	entt::entity m_drawingBoardId;

	std::string m_projectLocation;
	std::string m_projectName;
};