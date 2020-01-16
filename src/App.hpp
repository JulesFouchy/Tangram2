#pragma once

struct SDL_Window;
union SDL_Event;

#include "Instance.hpp"
#include <list>

class App {
private:
	App(SDL_Window* window);
	~App() = default;

	void addInstance();
	void switchInstance();
	Instance& activeInstance();

	void onWindowResize();
	void switchFullScreenMode();
	void handleEvents();

	inline void exit() { m_running = false; }

public:
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
