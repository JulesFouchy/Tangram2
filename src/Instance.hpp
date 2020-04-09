#pragma once

union SDL_Event;

#include <entt/entt.hpp>
#include <string>

#include "Input/InputSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/LayersManager.hpp"
#include "Shapes/ShapeFactory.hpp"

#include "Scripts/CellularLife/CellularLife.hpp"

class Instance {
public:
	Instance();
	Instance(const std::string& projectFolderpath);
	void Construct();
	~Instance();

	inline const std::string& getProjectName() { return m_projectName; }
	std::string getProjectPath();

	inline entt::registry& registry() { return m_registry; }
	inline RenderSystem& renderSystem() { return m_renderSystem; }
	inline InputSystem& inputSystem() { return m_inputSystem; }
	inline LayersManager& layersManager() { return m_layersManager; }

private:
	void onTransformMatrixChange(entt::entity e, entt::registry& R);
	void onMustRecomputeTexture(entt::entity e);

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

	entt::entity m_poly;
	entt::entity m_testLayer;
	entt::entity m_testLayer2;

	CellularLife m_cellularLife;

	std::string m_projectLocation;
	std::string m_projectName;
	bool m_bUserChoseProjectName;

	bool m_open = true; // used by the main tab bar to know when tab was closed and instance should be deleted
};