#include "App.hpp"

#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include "MyImGui/MyImGui.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/File.hpp"

#include "GUI/FileBrowser.hpp"

#include "Systems/GUISystem.hpp"

App* App::m_appInstance = nullptr;

App::App(SDL_Window* window)
	: m_bShowImGuiDemoWindow(false),
	  m_bFullScreen(false),
	  m_window(window), m_running(true)
{
	onWindowResize();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::addInstance() {
	m_instances.emplace_back();
	m_activeInstanceIt = --m_instances.end();
}

void App::addInstance(const std::string& folderpath) {
	m_instances.emplace_back(folderpath);
	m_activeInstanceIt = --m_instances.end();
}

void App::removeInstance(std::list<Instance>::iterator it) {
	if (it == m_activeInstanceIt)
		m_activeInstanceIt = m_instances.end();
	m_instances.erase(it);
}

void App::switchInstance() {
	m_activeInstanceIt++;
	if (m_activeInstanceIt == m_instances.end())
		m_activeInstanceIt = m_instances.begin();
}

Instance& App::activeInstance() { 
	return *m_activeInstanceIt;
}

std::list<Instance>::iterator App::itToInstanceWithPath(const std::string& path) {
	for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
		if (!std::filesystem::path(it->getProjectPath()).compare(std::filesystem::path(path)))
			return it;
	}
	return m_instances.end();
}

bool App::projectIsOpen(const std::string& projectPath) {
	return itToInstanceWithPath(projectPath) != m_instances.end();
}

void App::ImGui_InstancesWindow() {
	MyImGui::BeginInstancesMenu();

	std::list<Instance>::iterator toRemove = m_instances.end();
	if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs)) {
		for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
			if (ImGui::BeginTabItem(it->getProjectName().c_str(), &it->m_open, ImGuiTabItemFlags_None)) {
				m_activeInstanceIt = it;
				ImGui::EndTabItem();
			}
			if (!it->m_open)
				toRemove = it;
		}
		if (toRemove != m_instances.end())
			removeInstance(toRemove);
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void App::onWindowResize() {
	DisplayInfos::RefreshSize(m_window);
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
}

void App::switchFullScreenMode(){
	if (m_bFullScreen)
		SDL_SetWindowFullscreen(m_window, 0);
	else
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	m_bFullScreen = !m_bFullScreen;
	onWindowResize();
}

void App::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		bool bHandled = false;
		switch (e.type) {
		case SDL_KEYDOWN:
			//if (!ImGui::GetIO().WantCaptureKeyboard) {
				if (e.key.keysym.scancode == SDL_SCANCODE_F11) {
					switchFullScreenMode();
					bHandled = true;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen) {
					switchFullScreenMode();
					bHandled = true;
				}
				if (DisplayInfos::KeyIsDown(SDL_SCANCODE_LCTRL)) {
					if (e.key.keysym.scancode == SDL_SCANCODE_O) {
						std::string folderPath = FileBrowser::GetFolder();
						if (!folderPath.empty()) {
							auto it = itToInstanceWithPath(folderPath);
							if (it == m_instances.end())
								addInstance(folderPath);
							else
								m_activeInstanceIt = it;
						}
						bHandled = true;
					}
					else if (e.key.keysym.scancode == SDL_SCANCODE_N) {
						addInstance();
						bHandled = true;
					}
				}
			//}
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				onWindowResize();
				bHandled = true;
				break;
			}
			break;

		case SDL_QUIT:
			exit();
			bHandled = true;
			break;

		default:
			break;
		}
		if (!bHandled) {
			if (m_activeInstanceIt!=m_instances.end())
				activeInstance().onEvent(e);
		}
	}
}



void App::Initialize(SDL_Window* window) {
	assert(!m_appInstance);
	m_appInstance = new App(window);
	if (m_appInstance == nullptr)
		spdlog::error("[App::Initialize] Unable to allocate enough memory !");
	//m_appInstance->addInstance("C:\\Dev\\Tangram2\\MyTangramProjects\\TestPt2D");
	m_appInstance->addInstance();
}

void App::ShutDown() {
	delete m_appInstance;
}

void App::onLoopIteration() {
	glClearColor(0.5f, 0.75f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Events
	handleEvents();
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	if (GUISystem::ShouldShowGUI()) {
		// ImGui Debug and Demo
		#ifndef NDEBUG 
			ImGui::Begin("Debug");
			ImGui::Checkbox("Show Demo Window", &m_bShowImGuiDemoWindow);
			ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
			ImGui::End();
			if (m_bShowImGuiDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
				ImGui::ShowDemoWindow(&m_bShowImGuiDemoWindow);
		#endif
		// Main menu bar
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {
			ImGui::Text("Hello !");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			m_settings.ImGuiParameters();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		// Instances window
		ImGui_InstancesWindow();
		// Right side window
		MyImGui::RightSideWindow();
	}
	// Actual application code
	if (m_activeInstanceIt != m_instances.end())
		activeInstance().onLoopIteration();
	// Render ImGui
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	// End frame
	SDL_GL_SwapWindow(m_window);
}