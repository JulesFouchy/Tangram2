#pragma once

#include <glm/glm.hpp>

struct SDL_Window;

class DisplayInfos {
public:
	static void Initialize();
	static void RefreshSize(SDL_Window* window);

	static inline int Width()  { return m_windowWidth; }
	static inline int Height() { return m_windowHeight; }
	static inline float Ratio() { return m_aspectRatio; }
	static inline const glm::mat3& Matrix() { return m_windowMat; }

	static glm::vec2 MousePositionInPixels();
	static glm::vec2 MousePositionInScreen();
	static glm::vec2 MousePositionInNormalizedDeviceCoordinates();
	static glm::vec2 MousePositionInInches();

private:
	static int m_windowWidth;
	static int m_windowHeight;
	static float m_aspectRatio;
	static glm::mat3 m_windowMat;

	static float m_horizontalDPI;
	static float m_verticalDPI;
};