#pragma once

struct SDL_Window;
union SDL_Event;

#include "Instance.hpp"
#include <list>
#include <string>

class App {
friend class Window_SaveAsProject;
private:
	App(SDL_Window* window);
	~App() = default;

	void addInstance();
	void addInstance(const std::string& folderpath);
	void switchInstance();
	Instance& activeInstance();
	std::list<Instance>::iterator itToInstanceWithPath(const std::string& path);
	void ImGui_InstancesWindow();

	void onWindowResize();
	void switchFullScreenMode();
	void handleEvents();

	inline void exit() { m_running = false; }

public:
	bool projectIsOpen(const std::string& projectPath);

	static void Initialize(SDL_Window* window);
	static void ShutDown();

	inline static App& Get() { return *m_appInstance; }
	inline bool isRunning() const { return m_running; }
	void onLoopIteration();

private:
	std::list<Instance> m_instances;
	std::list<Instance>::iterator m_activeInstanceIt;

	bool m_bShowImGuiDemoWindow;
	bool m_bFullScreen;

	SDL_Window* m_window;
	bool m_running;
	static App* m_appInstance;
};
