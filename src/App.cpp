#include "App.hpp"

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Debugging/Log.hpp"
#include "Debugging/glException.hpp"

#include "Helper/DisplayInfos.hpp"

#include "Components/Translation.hpp"
#include "Components/Scale.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"

App::App(SDL_Window* window)
	: m_bShowImGUIDemoWindow(false),
	  m_bFullScreen(false),
	  m_registry(),
	  m_renderSystem(m_registry),
	  m_inputSystem(m_registry),
	  m_window(window), m_running(true)
{
	onWindowResize();
	createDrawingBoard();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::onInit() {
	auto id1 = m_registry.create();
	m_registry.assign<Cmp::Translation>(id1, 1.7f, 0.0f);
	m_registry.assign<Cmp::Scale>(id1, 0.3f);
	m_registry.assign<Cmp::AspectRatio>(id1, 1.5f);
	m_registry.assign<Cmp::Parent>(id1, m_drawingBoardId);

	auto id2 = m_registry.create();
	m_registry.assign<Cmp::Translation>(id2, 0.0f, 0.0f);
	m_registry.assign<Cmp::Scale>(id2, 3.0f);
	m_registry.assign<Cmp::AspectRatio>(id2, 1.0f);
	m_registry.assign<Cmp::Parent>(id2, m_drawingBoardId);
}

void App::onLoopIteration() {
	// ImGui windows
#ifndef NDEBUG 
	ImGui::Begin("Debug");
	ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
	ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();
	if (m_bShowImGUIDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShowImGUIDemoWindow);
#endif
	//
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderSystem.update();
	m_inputSystem.update();
}

void App::createDrawingBoard() {
	m_drawingBoardId = m_registry.create();
	m_registry.assign<Cmp::Translation>(m_drawingBoardId, 0.0f, 0.0f);
	m_registry.assign<Cmp::Scale>(m_drawingBoardId, 0.8f);
	m_registry.assign<Cmp::AspectRatio>(m_drawingBoardId, 16.0f / 9.0f);
}

void App::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		if (!ImGui::GetIO().WantCaptureMouse){

		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				m_inputSystem.onLeftClicDown();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				m_inputSystem.onRightClicDown();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				m_inputSystem.onLeftClicUp();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				m_inputSystem.onRightClicUp();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;


	case SDL_KEYDOWN:
		if (e.key.keysym.scancode == SDL_SCANCODE_F11)
			switchFullScreenMode();
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen)
			switchFullScreenMode();

		if (!ImGui::GetIO().WantCaptureKeyboard) {
			m_inputSystem.onKeyDown(e.key.keysym.scancode);
		}
		break;

	case SDL_KEYUP:
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			m_inputSystem.onKeyUp(e.key.keysym.scancode);
		}
		break;

	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			break;
		}
		break;

	case SDL_QUIT:
		exit();
		break;

	default:
		break;
	}
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

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL CODE /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

App* App::m_instance = nullptr;

void App::Initialize(SDL_Window* window) {
	assert(!m_instance);
	m_instance = new App(window);
	if (m_instance == nullptr)
		spdlog::error("[App::Initialize] Unable to allocate enough memory !");
}

void App::ShutDown() {
	delete m_instance;
}

void App::_loopIteration() {
	// Events
	handleSDLevents();
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	// Actual application code
	onLoopIteration();

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

void App::handleSDLevents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		onEvent(e);
	}
}