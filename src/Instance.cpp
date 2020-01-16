#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/AspectRatio.hpp"

#include "glm/gtx/matrix_transform_2d.hpp"

#include "Helper/DisplayInfos.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <imgui/imgui.h>

#include <cereal/archives/json.hpp>

Instance::Instance()
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this)
{
	createDrawingBoard();

	entt::entity id1;
	{
		entt::entity id = layersManager().addLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(id).val;
		mat = glm::translate(mat, glm::vec2(1.3f, 0.0f));
		mat = glm::scale(mat, glm::vec2(0.15f));
		registry().get<Cmp::AspectRatio>(id).val = 2.0f;
		id1 = id;
	}

	{
		entt::entity id = layersManager().addLayer();
		glm::mat3& mat = registry().get<Cmp::TransformMatrix>(id).val;
		mat = glm::translate(mat, glm::vec2(1.0f, 0.0f));
		mat = glm::scale(mat, glm::vec2(0.3f));
		registry().get<Cmp::Parent>(id).id = id1;
	}
}

void Instance::onLoopIteration(){
	glClear(GL_COLOR_BUFFER_BIT);
	renderSystem().render();
	inputSystem().update();
}

void Instance::createDrawingBoard() {
	m_drawingBoardId = registry().create();
	glm::mat3& mat = registry().assign<Cmp::TransformMatrix>(drawingBoardId()).val;
	mat = glm::scale(mat, glm::vec2(0.8f));
	mat = glm::rotate(mat, 0.1f);
	registry().assign<Cmp::AspectRatio>(drawingBoardId(), 16.0f / 9.0f);
}

glm::mat3 Instance::getMatrix(entt::entity id) {
	glm::mat3 model = registry().get<Cmp::TransformMatrix>(id).val;
	model = glm::scale(model, glm::vec2(registry().get<Cmp::AspectRatio>(id).val, 1.0f));
	return DisplayInfos::Matrix() * getParentModelMatrix(id) * model;
}

glm::mat3 Instance::getParentModelMatrix(entt::entity id) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(id);
	if (parent)
		return getParentModelMatrix(parent->id) * registry().get<Cmp::TransformMatrix>(parent->id).val;
	else
		return glm::mat3(1.0f);
}

void Instance::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		if (!ImGui::GetIO().WantCaptureMouse) {
			inputSystem().onWheelScroll((float)e.wheel.y);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				inputSystem().onLeftClicDown();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				inputSystem().onRightClicDown();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				inputSystem().onLeftClicUp();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				inputSystem().onRightClicUp();
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;


	case SDL_KEYDOWN:
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			inputSystem().onKeyDown(e.key.keysym.scancode);
		}
		break;

	case SDL_KEYUP:
		if (!ImGui::GetIO().WantCaptureKeyboard) {
			inputSystem().onKeyUp(e.key.keysym.scancode);
		}
		break;

	default:
		break;
	}
}