#include "Instance.hpp"

#include "Components/TransformMatrix.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/VisualDependencies.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Vertices.hpp"
#include "Components/Parameters.hpp"
#include "Components/Name.hpp"
#include "Components/Shader.hpp"
#include "Components/ShaderReference.hpp"
#include "Components/History.hpp"

#include "Core/MustRecomputeTexture.hpp"
#include "Core/GetFirstLayerRelatedTo.hpp"
#include "Core/GetDrawingBoard.hpp"

#include "Systems/ShaderSystem.hpp"
#include "Systems/GUISystem.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/String.hpp"
#include "Helper/File.hpp"

#include <cereal/archives/json.hpp>
#include <fstream>

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
	if (e != TNG::GetDrawingBoard(registry())) {
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
	  m_inputSystem(*this),
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
	//
	//layersManager().createFragmentLayer("res/shaders/vasarely.frag");
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
	layersManager().createPolygonLayer(registry(), { glm::vec2(-0.3, -0.5), glm::vec2(0, 0), glm::vec2(0.8, -0.5), glm::vec2(-0.8, -0.5), glm::vec2(0.8, 0.5) });
	//renderSystem().computeTexture_Polygon(m_poly, 32.0f);
}


Instance::Instance(const std::string& projectFolderpath)
	: m_registry(),
	  m_inputSystem(*this),
	  m_bUserChoseProjectName(true)
{
	Construct();
	openProject(projectFolderpath);
}

void Instance::onLoopIteration(){
	renderSystem().render(m_registry, m_layersManager.getLayersOrdered(), m_layersManager.getSelectedLayer(), GUISystem::ShouldShowGUI());
	renderSystem().checkTexturesToRecompute(m_registry);
	inputSystem().update();
	GUISystem::Render(m_registry, m_layersManager.getLayersOrdered(), m_layersManager.selectedLayer());
}

void Instance::createDrawingBoard() {
	entt::entity e = registry().create();
	glm::mat3 mat(1.0f);
	mat = glm::scale(mat, glm::vec2(0.8f));
	//mat = glm::rotate(mat, 0.1f);
	registry().assign<entt::tag<"DrawingBoard"_hs>>(e);
	registry().assign<Cmp::TransformMatrix>(e, mat);
	registry().assign<Cmp::AspectRatio>(e, 1.0f);
	registry().assign<Cmp::Children>(e);
	registry().assign<Cmp::Texture>(e, Settings::GetPREVIEW_SIZE_IN_PX(), Settings::GetPREVIEW_SIZE_IN_PX());
	registry().assign<Cmp::History>(e);
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
				entt::entity clickedEntity = layersManager().getEntityHoveredByMouse(registry(), renderSystem());
				entt::entity clickedLayer = TNG::GetFirstLayerRelatedTo(registry(), clickedEntity);
				inputSystem().onLeftClicDown(clickedEntity, clickedLayer, layersManager().selectedLayer());
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
			CEREAL_NVP(m_layersManager)
		);
		cereal::JSONOutputArchive registryArchive(registryOs);
		registry().snapshot()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component<Cmp::AspectRatio, Cmp::Children, Cmp::Parent, Cmp::Texture, Cmp::TransformMatrix, Cmp::Vertices, Cmp::VisualDependencies,
			Cmp::Name, Cmp::Shader, Cmp::ShaderReference,
			Cmp::History,
			entt::tag<"DrawingBoard"_hs>,
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
			m_layersManager
		);
		cereal::JSONInputArchive registryArchive(registryIs);
		registry().loader()
			.entities(registryArchive)
			.destroyed(registryArchive)
			.component < Cmp::AspectRatio, Cmp::Children, Cmp::Parent, Cmp::Texture, Cmp::TransformMatrix, Cmp::Vertices, Cmp::VisualDependencies,
			Cmp::Name, Cmp::Shader, Cmp::ShaderReference,
			Cmp::History,
			entt::tag<"DrawingBoard"_hs>,
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