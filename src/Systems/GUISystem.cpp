#include "GUISystem.hpp"

#include "Instance.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Components/ParametersList.hpp"
#include "Components/GUI/SliderFloat.hpp"
#include "Components/GUI/SliderFloat2.hpp"
#include "Components/GUI/ColorPicker3.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Name.hpp"

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
		ImGui::BeginGroup();
		if (ImGui::Selectable(("##layer" + std::to_string(k)).c_str(), layer == I.layersManager().m_selectedLayer, 0, ImVec2(0, height))) {
			I.layersManager().m_selectedLayer = layer;
		}
		//
		ImGui::SameLine();
		ImGui::Image((ImTextureID)tex.id, ImVec2(height * aspect, height), ImVec2(0, 1), ImVec2(1, 0),
			ImVec4(1, 1, 1, 1), ImVec4(0.6, 0.6, 0.6, 1));
		ImGui::SameLine();
		//ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
		//static std::string str = "sdf";
		//if (layer == I.layersManager().m_selectedLayer)
		//	ImGui::InputText("", &str);
		//else
			ImGui::Text(I.registry().get<Cmp::Name>(layer).val.c_str());
		//ImGui::PopStyleVar();
		ImGui::EndGroup();
		//if (ImGui::IsItemActivated()) {
		//	spdlog::error("activating");
		//	draggingLayer = true;
		//	swapWith = k;
		//}
		//if (ImGui::IsItemDeactivated()) {
		//	spdlog::error("deactivating");
		//	draggingLayer = false;
		//}
		////if (ImGui::IsItemHovered())
		////	spdlog::warn("hovered {}", k);
		//if (draggingLayer && ImGui::IsItemHovered()) {
		//	I.layersManager().swapLayers(k, swapWith);
		//}
		//if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		//{
		//	int k_next = k + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		//	if (k_next >= 0 && k_next < I.layersManager().getLayersOrdered().size())
		//	{
		//		spdlog::info("swap");
		//		I.layersManager().swapLayers(k, k_next);
		//		//mustSwap = true;
		//		//swap1 = k; swap2 = k_next;
		//		ImGui::ResetMouseDragDelta();
		//	}
		//}
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
			bool bMustRecomputeTexture = false;
			for (const auto& param : R.get<Cmp::Parameters>(selLayer).list) {
				bMustRecomputeTexture |= param->ImGui();
			}
			if (bMustRecomputeTexture)
				R.assign<entt::tag<"MustRecomputeTexture"_hs>>(selLayer);
			ImGui::End();
		}
	}
}