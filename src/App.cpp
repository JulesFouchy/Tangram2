#include "App.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Helper/DisplayInfos.hpp"

#include "Debugging/Log.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

App* App::m_appInstance = nullptr;

App::App(SDL_Window* window)
	: m_bShowImGuiDemoWindow(false),
	  m_bFullScreen(false),
	  m_window(window), m_running(true)
{
	onWindowResize();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	addInstance();
	m_activeInstanceIt = m_instances.begin();
}

void App::addInstance() {
	m_instances.emplace_back();
}

void App::switchInstance() {
	m_activeInstanceIt++;
	if (m_activeInstanceIt == m_instances.end())
		m_activeInstanceIt = m_instances.begin();
}

Instance& App::activeInstance() { 
	return *m_activeInstanceIt;
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
			if (!ImGui::GetIO().WantCaptureKeyboard) {
				if (e.key.keysym.scancode == SDL_SCANCODE_F11) {
					switchFullScreenMode();
					bHandled = true;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen) {
					switchFullScreenMode();
					bHandled = true;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_TAB) {
					switchInstance();
					bHandled = true;
				}
			}
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
		if (!bHandled)
			activeInstance().onEvent(e);
	}
}



void App::Initialize(SDL_Window* window) {
	assert(!m_appInstance);
	m_appInstance = new App(window);
	if (m_appInstance == nullptr)
		spdlog::error("[App::Initialize] Unable to allocate enough memory !");
}

void App::ShutDown() {
	delete m_appInstance;
}

void App::onLoopIteration() {
	// Events
	handleEvents();
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	// ImGui Debug and Demo
#ifndef NDEBUG 
	ImGui::Begin("Debug");
	ImGui::Checkbox("Show Demo Window", &m_bShowImGuiDemoWindow);
	ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();
	if (m_bShowImGuiDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShowImGuiDemoWindow);
#endif
	// Actual application code
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