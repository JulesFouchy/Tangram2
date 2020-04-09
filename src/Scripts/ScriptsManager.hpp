#pragma once

class Script;
class LayersManager;

class ScriptsManager {
public:
	ScriptsManager() = default;
	void initialize(entt::registry& R, LayersManager& layersM);
	~ScriptsManager();

	void loopIteration(entt::registry& R, float dt);
	void ImGui(entt::registry& R);

private:
	std::vector<Script*> m_scripts;
};