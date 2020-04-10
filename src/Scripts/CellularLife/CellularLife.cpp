#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

#include "Components/Parameters.hpp"

#include "Core/GetPosition.hpp"
#include "Core/SetPosition.hpp"

#include "Helper/DisplayInfos.hpp"
#include "Helper/Random.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <fstream>

#include "GUI/FileBrowser.hpp"

static float attractionStrengthRange[2] = { -10.0f , 10.0f };
static float attractionDistanceRange[2] = { 0.0f , 1.0f };
static float repulsionStrengthRange[2] = { 0.0f, 100.0f };
static float repulsionDistanceRange[2] = { 0.0f, 0.4f };

CellularLife::CellularLife(entt::registry& R, LayersManager& layersM)
	: m_dampingCoef(8.645f), m_maxRadius(0.82f)
{
	layersM.createFragmentLayer(R, "res/shaders/CellularLife_Background.frag");
	m_layer = layersM.createFragmentLayer(R, "res/shaders/CellularLife_Cells.frag");
	std::vector<Point2DParameter>& pts = getPointsList(R);
	for (Point2DParameter& pt : pts) {
		entt::entity e = pt.getEntity();
		m_cells.emplace_back(m_rand, e);
	}
	resetPositions(R);
	randomizeTypesDistribution();
	//
	m_multipliers = { 1.0f, 1.0f, 1.0f, 1.0f };
	randomizeSettings();
}

void CellularLife::randomizeTypesDistribution() {
	Log::separationLine();
	for (Cell cell : m_cells) {
		cell.m_typeID = m_rand.Int(NB_TYPES);
	}
}

void CellularLife::randomizeSettings() {
	for (size_t i = 0; i < NB_TYPES; ++i) {
		for (size_t j = 0; j < NB_TYPES; ++j) {
			m_settings[i][j].attractionStrengthMax = m_rand.Float(attractionStrengthRange[0], attractionStrengthRange[1]);
			m_settings[i][j].attractionDistanceMax = m_rand.Float(attractionDistanceRange[0], attractionDistanceRange[1]);
			m_settings[i][j].repulsionStrengthMax  = m_rand.Float(repulsionStrengthRange [0], repulsionStrengthRange [1]);
			m_settings[i][j].repulsionDistanceMax = 0.2f;// m_rand.Float(repulsionDistanceRange[0], repulsionDistanceRange[1]);

		}
	}
}

void CellularLife::saveSettings() {
	std::ofstream os("C:\\Dev\\Tangram2\\MyTangramProjects\\CellsForAmanthilde\\settings.json");
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_settings),
			CEREAL_NVP(m_multipliers)
		);
	}
}

void CellularLife::loadSettings() {
	const std::string path = FileBrowser::GetFileOpen();
	if (!path.empty()) {
		std::ifstream is(path);
		{
			cereal::JSONInputArchive archive(is);
			archive(
				m_settings,
				m_multipliers
			);
		}
	}
}

void CellularLife::resetPositions(entt::registry& R) {
	float ratio = DisplayInfos::Ratio();
	for (Cell& cell : m_cells)
		cell.setPosition(R, glm::vec2((m_rand.Float()-0.5f)*ratio, m_rand.Float()-0.5f));
}

void CellularLife::loopIteration(entt::registry& R, float dt) {
	checkEntityValidity(R);
	applyInteractions(R, dt);
	for (Cell& cell : m_cells) {
		cell.applyDamping(dt, m_dampingCoef);
		cell.move(R, dt, m_maxRadius);
	}
}

glm::vec2 CellularLife::computeForce(glm::vec2 p1, glm::vec2 p2, unsigned int id1, unsigned int id2) {
	InteractionSettings settings = m_settings[id1][id2];

	float force;
	float d = glm::distance(p1, p2);
	float r = settings.repulsionDistanceMax * m_multipliers.repulsionDistanceMax;
	float repStrength = settings.repulsionStrengthMax * m_multipliers.repulsionStrengthMax;
	float attDistance = settings.attractionDistanceMax * m_multipliers.attractionDistanceMax;
	float attStrength = settings.attractionStrengthMax * m_multipliers.attractionStrengthMax;

	if (d < r) {
		force = (sqrt(d / r) - 1.0f) * repStrength;
	}
	else {
		d -= r;
		if (d < attDistance / 2.0f) {
			force = d * attStrength / attDistance * 2.0f;
		}
		else {
			d -= attDistance / 2.0f;
			if (d < attDistance / 2.0f) {
				force = attStrength - d * attStrength / attDistance * 2.0f;
			}
			else
				force = 0.0f;
		}
	}
	//
	return force * glm::normalize(p2 - p1);
}

void CellularLife::applyInteractions(entt::registry& R, float dt) {
	for (size_t i = 0; i < m_cells.size(); ++i) {
		for (size_t j = i + 1; j < m_cells.size(); ++j) {
			glm::vec2 p1 = m_cells[i].getPosition(R);
			glm::vec2 p2 = m_cells[j].getPosition(R);
			unsigned int id1 = m_cells[i].getTypeID();
			unsigned int id2 = m_cells[j].getTypeID();
			
			m_cells[i].applyForce(dt, computeForce(p1, p2, id1, id2));
			m_cells[j].applyForce(dt, computeForce(p2, p1, id2, id1));
		}
	}
}

void CellularLife::ImGui(entt::registry& R) {
	ImGui::Begin("Cellular Life");
	if (ImGui::Button("Reset Positions")) {
		resetPositions(R);
	}
	if (ImGui::Button("Randomize Interaction Settings")) {
		randomizeSettings();
	}
	if (ImGui::Button("Randomize Types Distribution")) {
		randomizeTypesDistribution();
	}
	ImGui::SliderFloat("Damping Coef", &m_dampingCoef, 0.0f, 13.0f);
	ImGui::SliderFloat("Container Radius", &m_maxRadius, 0.8f, 1.0f);
	ImGui::Separator();
	ImGui::SliderFloat("Strength Attraction Mult", &m_multipliers.attractionStrengthMax, 0.0f, 3.0f);
	ImGui::SliderFloat("Distance Attraction Mult", &m_multipliers.attractionDistanceMax, 0.0f, 3.0f);
	ImGui::SliderFloat("Strength Repulsion Mult",  &m_multipliers.repulsionStrengthMax,  0.0f, 3.0f);
	ImGui::SliderFloat("Distance Replusion Mult",  &m_multipliers.repulsionDistanceMax,  0.0f, 3.0f);
	for (size_t i = 0; i < NB_TYPES; ++i) {
		for (size_t j = 0; j < NB_TYPES; ++j) {
			ImGui::Separator();
			ImGui::TextColored({ 0.8, 0.3, 0.2, 1.0 }, (std::to_string(i) + "->" + std::to_string(j)).c_str());
			ImGui::PushID(i * NB_TYPES + j);
			ImGui::SliderFloat("Strength Attraction Max", &m_settings[i][j].attractionStrengthMax, attractionStrengthRange[0], attractionStrengthRange[1]);
			ImGui::SliderFloat("Distance Attraction Max", &m_settings[i][j].attractionDistanceMax, attractionDistanceRange[0], attractionDistanceRange[1]);
			ImGui::SliderFloat("Strength Repulsion Max", &m_settings[i][j].repulsionStrengthMax, repulsionStrengthRange[0], repulsionStrengthRange[1]);
			ImGui::SliderFloat("Distance Replusion Max", &m_settings[i][j].repulsionDistanceMax, repulsionDistanceRange[0], repulsionDistanceRange[1]);
			ImGui::PopID();
		}
	}
	ImGui::Separator();
	if (ImGui::Button("Save Interaction Settings")) {
		saveSettings();
	}

	if (ImGui::Button("Load Interaction Settings")) {
		loadSettings();
	}
	ImGui::End();
}

std::vector<Point2DParameter>& CellularLife::getPointsList(entt::registry& R) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(m_layer);
	return ((ListOfPoints2DParameter*)params.list[0].get())->getList();
}

void CellularLife::checkEntityValidity(entt::registry& R) {
	if (!R.valid(m_cells[0].m_entity)) {
		std::vector<Point2DParameter>& pts = getPointsList(R);
		int i = 0;
		for (Point2DParameter& pt : pts) {
			entt::entity e = pt.getEntity();
			if (i < m_cells.size())
				m_cells[i].m_entity = e;
			else {
				m_cells.emplace_back(m_rand, e);
			}
			i++;
		}
	}
	std::vector<Point2DParameter>& pts = getPointsList(R);
	if (pts.size() < m_cells.size()) {
		m_cells.resize(pts.size());
	}
}