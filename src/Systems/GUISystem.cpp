#include "GUISystem.hpp"

#include "Instance.hpp"

#include <imgui/imgui.h>

#include "Components/ParametersList.hpp"
#include "Components/GUI/SliderFloat.hpp"
#include "Components/GUI/ColorPicker3.hpp"
#include "Components/AspectRatio.hpp"

bool GUISystem::s_bShowGUI = true;

GUISystem::GUISystem(Instance& instance)
	: ISystem(instance)
{}

void GUISystem::LayersWindow() {
	float height = 40.0f;
	ImGui::Begin("Layers");
	int k = 0;
	for (entt::entity layer : I.layersManager().getLayersOrdered()) {
		Cmp::Texture& tex = I.registry().get<Cmp::Texture>(layer);
		float aspect = I.registry().get<Cmp::AspectRatio>(I.drawingBoardId()).val;
		if (ImGui::Selectable(("##layer" + std::to_string(k)).c_str(), layer == I.layersManager().m_selectedLayer, 0, ImVec2(0, height))) {
			I.layersManager().m_selectedLayer = layer;
		}
		ImGui::SameLine();
		ImGui::Image((ImTextureID)tex.id, ImVec2(height * aspect, height), ImVec2(0, 1), ImVec2(1, 0),
			ImVec4(1, 1, 1, 1), ImVec4(0.6,0.6,0.6, 1));
		ImGui::SameLine();                    
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
		ImGui::Selectable(("Layer" + std::to_string(k)).c_str(), false, 0, ImVec2(0, height));
		ImGui::PopStyleVar();
		ImGui::PushID(k);
		ImGui::PopID();
		k++;
	}
	ImGui::End();
}

void GUISystem::render() {
	if (s_bShowGUI) {
		LayersWindow();
		//
		entt::registry& R = I.registry();
		entt::entity selLayer = I.layersManager().selectedLayer();
		if (I.registry().valid(selLayer)) {
			ImGui::Begin("Parameters");
			for (entt::entity guiElement : R.get<Cmp::Parameters>(selLayer).list) {
				// Slider float
				Cmp::SliderFloat* sf = R.try_get<Cmp::SliderFloat>(guiElement);
				if (sf)
					if (ImGui::SliderFloat(sf->name.c_str(), &sf->val, sf->minVal, sf->maxVal, sf->format.c_str(), sf->power))
						R.assign<entt::tag<"MustRecomputeTexture"_hs>>(selLayer);
					else {
						// ColorPicker3
						Cmp::ColorPicker3* cp3 = R.try_get<Cmp::ColorPicker3>(guiElement);
						if (cp3)
							ImGui::ColorPicker3(cp3->name.c_str(), (float*)&cp3->val, cp3->flags);
					}
			}
			ImGui::End();
		}
	}
}