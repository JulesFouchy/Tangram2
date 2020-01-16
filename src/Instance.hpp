#pragma once

union SDL_Event;

#include <entt/entt.hpp>

#include "Render/RenderSystem.hpp"
#include "Input/InputSystem.hpp"
#include "Layers/LayersManager.hpp"

class Instance {
public:
	Instance();
	~Instance() = default;

	glm::mat3 getMatrix(entt::entity id, bool bIncludeRatio = true);
	glm::mat3 getParentModelMatrix(entt::entity id);

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

private:
	entt::registry m_registry;
	RenderSystem m_renderSystem;
	InputSystem m_inputSystem;
	LayersManager m_layersManager;
	entt::entity m_drawingBoardId;
};