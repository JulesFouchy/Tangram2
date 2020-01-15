#include "DisplayInfos.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/gtx/matrix_transform_2d.hpp>

int DisplayInfos::m_windowWidth;
int DisplayInfos::m_windowHeight;
float DisplayInfos::m_aspectRatio;
glm::mat3 DisplayInfos::m_windowMat;

void DisplayInfos::RefreshSize(SDL_Window* window) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	m_windowWidth = w;
	m_windowHeight = h;
	m_aspectRatio = w / (float) h;
	m_windowMat = glm::scale(glm::mat3(1.0f), glm::vec2(1.0f/m_aspectRatio, 1.0f));
}