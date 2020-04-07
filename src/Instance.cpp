#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/VisualDependencies.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Vertices.hpp"
#include "Components/ParametersList.hpp"
#include "Components/Name.hpp"
#include "Components/Shader.hpp"
#include "Components/ShaderReference.hpp"
#include "Components/History.hpp"

#include "Core/MustRecomputeTexture.hpp"

#include "Systems/ShaderSystem.hpp"

#include "glm/gtx/matrix_transform_2d.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/String.hpp"
#include "Helper/File.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <imgui/imgui.h>

#include <cereal/archives/json.hpp>
#include <fstream>
#include "Debugging/Log.hpp"

#include "App.hpp"

static void deleteTexture(entt::entity e, entt::registry& R) {
	R.get<Cmp::Texture>(e).Delete();
}
static void deleteShader(entt::entity e, entt::registry& R) {
	R.get<Cmp::Shader>(e).Delete();
}
//static void deleteParameters(entt::entity e, entt::registry& R) {
//	for (Parameter* ptr : R.get<Cmp::Parameters>(e).list)
//		delete ptr;
//}

void Instance::onTransformMatrixChange(entt::entity e, entt::registry& R) {
	if (e != drawingBoardId()) {
		onMustRecomputeTexture(e);
		Cmp::Children& children = R.get<Cmp::Children>(e);
		for (entt::entity child : children.list) {
			if (R.valid(child))
				onTransformMatrixChange(child, R);
		}
	}
}

void Instance::onMustRecomputeTexture(entt::entity e) {
	TNG::MustRecomputeTexture(registry(), e);
	Cmp::VisualDependencies* dependencies = registry().try_get<Cmp::VisualDependencies>(e);
	if (dependencies) {
		for (entt::entity dependant : dependencies->list)
			onMustRecomputeTexture(dependant);
	}
}

void Instance::Construct() {
	// Events
	//registry().on_construct<Cmp::TransformMatrix>().connect<&Instance::onTransformMatrixChange>(*this);
	registry().on_replace<Cmp::TransformMatrix>().connect<&Instance::onTransformMatrixChange>(*this);
	registry().on_replace<Cmp::Texture>().connect<&deleteTexture>();
	registry().on_destroy<Cmp::Texture>().connect<&deleteTexture>();
	registry().on_replace<Cmp::Shader>().connect<&deleteShader>();
	registry().on_destroy<Cmp::Shader>().connect<&deleteShader>();
	//registry().on_destroy<Cmp::Parameters>().connect<&deleteParameters>();
}

Instance::~Instance() {
	{
		auto view = m_registry.view<Cmp::Texture>();
		m_registry.destroy(view.begin(), view.end()); // make sure that the "destructor" of textures is called
	}
	{
		auto view = m_registry.view<Cmp::Shader>();
		m_registry.destroy(view.begin(), view.end()); // make sure that the "destructor" of shaders is called
	}
}

Instance::Instance()
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this),
	  m_guiSystem(*this),
	  m_projectLocation(MyFile::RootDir+"/MyTangramProjects"),
	  m_bUserChoseProjectName(false)
{
	Construct();
	// Project default name
	m_projectName = "UntitledProject0";
	int k = 1;
	while (MyFile::Exists(getProjectPath()) || App::Get().projectIsOpen(getProjectPath())) {
		m_projectName = "UntitledProject" + std::to_string(k++);
	}
	// Drawing board
	createDrawingBoard();
	m_cellularLife = CellularLife(registry(), m_layersManager);
	//
	//layersManager().createFragmentLayer("res/shaders/myFirstFrag.frag");
	//layersManager().createFragmentLayer("res/shaders/second.frag");
	//layersManager().createFragmentLayer("res/shaders/myFirstFrag.frag");
	//m_shapeFactory.createPolygon({ glm::vec2(-0.3, -0.5), glm::vec2(0, 0), glm::vec2(0.8, -0.5), glm::vec2(-0.8, -0.5), glm::vec2(0.8, 0.5) });
	//
	{
		//m_testLayer2 = layersManager().createTestLayer();
		//glm::mat3 mat = registry().get<Cmp::TransformMatrix>(m_testLayer2).val();
		//mat = glm::translate(mat, glm::vec2(1.3f, 0.0f));
		//mat = glm::scale(mat, glm::vec2(0.85f));
		//registry().replace<Cmp::TransformMatrix>(m_testLayer2, mat);
		//registry().get<Cmp::AspectRatio>(id).val = 2.0f;
	}
	
	{
		//m_testLayer = layersManager().createTestLayer();
		//glm::mat3 mat = registry().get<Cmp::TransformMatrix>(m_testLayer).val();
		//mat = glm::translate(mat, glm::vec2(1.0f, 0.0f));
		//mat = glm::scale(mat, glm::vec2(0.5f));
		//registry().replace<Cmp::TransformMatrix>(m_testLayer, mat);
		//setParentOf(m_testLayer, m_testLayer2);
	}
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createTestLayer();
	//layersManager().createPolygonLayer({ glm::vec2(-0.3, -0.5), glm::vec2(0, 0), glm::vec2(0.8, -0.5), glm::vec2(-0.8, -0.5), glm::vec2(0.8, 0.5) });
	//renderSystem().computeTexture_Polygon(m_poly, 32.0f);
}


Instance::Instance(const std::string& projectFolderpath)
	: m_registry(),
	  m_renderSystem(*this),
	  m_inputSystem(*this),
	  m_layersManager(*this),
	  m_guiSystem(*this),
	  m_bUserChoseProjectName(true)
{
	Construct();
	openProject(projectFolderpath);
}

void Instance::onLoopIteration(){
	renderSystem().render();
	renderSystem().checkTexturesToRecompute();
	inputSystem().update();
	guiSystem().render(); 
	m_cellularLife.loopIteration(1./60, registry());
	m_cellularLife.ImGui(registry());
}

void Instance::createDrawingBoard() {
	m_drawingBoardId = registry().create();
	glm::mat3 mat(1.0f);
	mat = glm::scale(mat, glm::vec2(0.8f));
	//mat = glm::rotate(mat, 0.1f);
	registry().assign<Cmp::TransformMatrix>(drawingBoardId(), mat);
	registry().assign<Cmp::AspectRatio>(drawingBoardId(), 1.0f);
	registry().assign<Cmp::Children>(drawingBoardId());
	registry().assign<Cmp::Texture>(drawingBoardId(), 1000, 1000);
	registry().assign<Cmp::History>(drawingBoardId());
}

glm::mat3 Instance::getLocalTransform(entt::entity e) {
	return registry().get<Cmp::TransformMatrix>(e).val();
}

glm::mat3 Instance::getMatrix(entt::entity e) {
	glm::mat3 model = getLocalTransform(e);
	return DisplayInfos::Matrix() * getParentModelMatrix(e) * model;
}

glm::mat3 Instance::getMatrixPlusAspectRatio(entt::entity e) {
	glm::mat3 model = getLocalTransform(e);
	Cmp::AspectRatio* ratio = registry().try_get<Cmp::AspectRatio>(e);
	if (ratio)
		model = glm::scale(model, glm::vec2(ratio->val, 1.0f));
	return DisplayInfos::Matrix() * getParentModelMatrix(e) * model;
}

glm::mat3 Instance::getMatrixToDBSpace(entt::entity e) {
	glm::mat3 model = getLocalTransform(e);
	return getParentModelMatrixExcludingDB(e) * model;
}

glm::mat3 Instance::getMatrixToTextureSpace(entt::entity e) {
	glm::mat3 mat = glm::inverse(getMatrixToDBSpace(e));
	float DBratio = registry().get<Cmp::AspectRatio>(drawingBoardId()).val;
	mat = glm::scale(mat, glm::vec2(2.0f*DBratio, 2.0f));
	mat = glm::translate(glm::mat3(1.0f), -glm::vec2(DBratio, 1.0f)) * mat;
	return mat;
}

glm::mat3 Instance::getParentModelMatrix(entt::entity e) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(e);
	if (parent)
		return getParentModelMatrix(parent->id) * getLocalTransform(parent->id);
	else
		return glm::mat3(1.0f);
}

glm::mat3 Instance::getParentModelMatrixExcludingDB(entt::entity e) {
	Cmp::Parent* parent = registry().try_get<Cmp::Parent>(e);
	if (parent && parent->id != drawingBoardId())
		return getParentModelMatrixExcludingDB(parent->id) * getLocalTransform(parent->id);
	else
		return glm::mat3(1.0f);
}

std::string Instance::getProjectPath() {
	return m_projectLocation + "/" + (m_projectName.empty() ? "UntitledProject" : m_projectName);
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
				inputSystem().onWheelDown();
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
				inputSystem().onWheelUp();
			}
		}
		break;


	case SDL_KEYDOWN:
		//if (!ImGui::GetIO().WantCaptureKeyboard)
			inputSystem().onKeyDown(e.key.keysym.scancode);
		break;

	case SDL_KEYUP:
		//if (!ImGui::GetIO().WantCaptureKeyboard)
			inputSystem().onKeyUp(e.key.keysym.scancode);
		break;

	default:
		break;
	}
}

void Instance::saveProject(const std::string& folderpath) {
	m_projectLocation = MyString::GetFolderHierarchy(folderpath);
	m_projectName = MyString::RemoveFolderHierarchy(folderpath);

	spdlog::info("Saving project to '{}'", folderpath);
	std::ofstream registryOs(folderpath+"/reg.tng");
	std::ofstream otherOs(folderpath + "/other.tng");
	{
		cereal::JSONOutputArchive otherArchive(otherOs);
		otherArchive(
			CEREAL_NVP(m_layersManager),
			CEREAL_NVP(m_drawingBoardId)
		);
		cereal::JSONOutputArchive registryArchive(registryOs);
		registry().snapshot()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component<Cmp::AspectRatio, Cmp::Children, Cmp::Parent, Cmp::Texture, Cmp::TransformMatrix, Cmp::Vertices, Cmp::VisualDependencies,
			Cmp::Name, Cmp::Shader, Cmp::ShaderReference,
			Cmp::History,
			entt::tag<"Point2D"_hs>, entt::tag<"Layer"_hs>,
			entt::tag<"Polygon"_hs>, entt::tag<"TestLayer"_hs>, entt::tag<"FragmentLayer"_hs>,
			entt::tag<"SaveMeInTheHistoryOfMyParentsParameters"_hs>,
			Cmp::Parameters>(registryArchive);
	}
	Log::separationLine();
}

void Instance::openProject(const std::string& folderpath) {
	m_projectLocation = MyString::GetFolderHierarchy(folderpath);
	m_projectName = MyString::RemoveFolderHierarchy(folderpath);

	spdlog::info("Opening project from '{}'", folderpath);
	std::ifstream registryIs(folderpath + "/reg.tng");
	std::ifstream otherIs(folderpath + "/other.tng");
	{
		cereal::JSONInputArchive otherArchive(otherIs);
		otherArchive(
			m_layersManager,
			m_drawingBoardId
		);
		cereal::JSONInputArchive registryArchive(registryIs);
		registry().loader()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component < Cmp::AspectRatio, Cmp::Children, Cmp::Parent, Cmp::Texture, Cmp::TransformMatrix, Cmp::Vertices, Cmp::VisualDependencies,
			Cmp::Name, Cmp::Shader, Cmp::ShaderReference,
			Cmp::History,
			entt::tag<"Point2D"_hs>, entt::tag<"Layer"_hs>,
			entt::tag<"Polygon"_hs>, entt::tag<"TestLayer"_hs>, entt::tag<"FragmentLayer"_hs>,
			entt::tag<"SaveMeInTheHistoryOfMyParentsParameters"_hs>,
			Cmp::Parameters>(registryArchive);
	}
	// Compute textures
	auto& layersWithPrevTexture = registry().view<Cmp::Texture>();
	for (entt::entity e : layersWithPrevTexture)
		registry().assign<entt::tag<"MustRecomputeTexture"_hs>>(e);
	// For Point2DParameter : Give a reference to the registry
	auto& params = registry().view<Cmp::Parameters>();
	for (entt::entity e : params)
		for (std::shared_ptr<Parameter> param : registry().get<Cmp::Parameters>(e).list)
			param->initializeRegistry(registry());
	// Compile Cmp::Shader
	auto& layersWithShader = registry().view<Cmp::ShaderReference>();
	for (entt::entity e : layersWithShader)
		ShaderSystem::CompileShaderAndUpdateParametersList(registry(), e);
	//
	Log::separationLine();
}