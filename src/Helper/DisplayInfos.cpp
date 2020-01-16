#include "DisplayInfos.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/gtx/matrix_transform_2d.hpp>


int DisplayInfos::m_windowWidth;
int DisplayInfos::m_windowHeight;
float DisplayInfos::m_aspectRatio;
glm::mat3 DisplayInfos::m_windowMat;

float DisplayInfos::m_horizontalDPI;
float DisplayInfos::m_verticalDPI;


void DisplayInfos::Initialize() {
	SDL_GetDisplayDPI(0, nullptr, &m_horizontalDPI, &m_verticalDPI);
}

void DisplayInfos::RefreshSize(SDL_Window* window) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	m_windowWidth = w;
	m_windowHeight = h;
	m_aspectRatio = w / (float) h;
	m_windowMat = glm::scale(glm::mat3(1.0f), glm::vec2(1.0f/m_aspectRatio, 1.0f));
}

glm::vec2 DisplayInfos::MousePositionInPixels() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(x, DisplayInfos::Height() - y);
}

glm::vec2 DisplayInfos::MousePositionInScreen() {
	return (MousePositionInPixels() / glm::vec2(DisplayInfos::Height(), DisplayInfos::Height()) - glm::vec2(0.5f)) * 2.0f;
}

glm::vec2 DisplayInfos::MousePositionInNormalizedDeviceCoordinates() {
	return (MousePositionInPixels() / glm::vec2(DisplayInfos::Width(), DisplayInfos::Height()) - glm::vec2(0.5f)) * 2.0f;
}

glm::vec2 DisplayInfos::MousePositionInInches() {
	return MousePositionInPixels() / glm::vec2(m_horizontalDPI, m_verticalDPI);
}

bool DisplayInfos::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}