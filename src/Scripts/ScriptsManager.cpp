#include "ScriptsManager.hpp"

#include "Script.hpp"
#include "CellularLife/CellularLife.hpp"

#include "Systems/LayersManager.hpp"

void ScriptsManager::initialize(entt::registry& R, LayersManager& layersM) {
	m_scripts.push_back((Script*)new CellularLife(R, layersM));
}

ScriptsManager::~ScriptsManager() {
	for (Script* ptr : m_scripts)
		delete ptr;
}

void ScriptsManager::loopIteration(entt::registry& R, float dt) {
	for (Script* script : m_scripts)
		script->loopIteration(R, dt);
}

void ScriptsManager::ImGui(entt::registry& R) {
	for (Script* script : m_scripts)
		script->ImGui(R);
}