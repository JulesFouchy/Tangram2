#include "GUISystem.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Components/ParametersList.hpp"
#include "Components/AspectRatio.hpp"
#include "Components/Name.hpp"
#include "Components/PreviewTexture.hpp"
#include "Components/AspectRatio.hpp"

#include "Core/MustRecomputeTexture.hpp"
#include "Core/ChangeActiveHistory.hpp"

bool GUISystem::s_bShowGUI = true;

void GUISystem::LayersWindow(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& rSelectedLayer, entt::entity drawingBoardEntity) {
	float height = 40.0f;
	ImGui::Begin("Layers");
	int k = 0;
	for (entt::entity layer : layersOrdered) {
		Cmp::Texture& tex = R.get<Cmp::Texture>(layer);
		float aspect = R.get<Cmp::AspectRatio>(drawingBoardEntity).val;
		ImGui::BeginGroup();
		if (ImGui::Selectable(("##layer" + std::to_string(k)).c_str(), layer == rSelectedLayer, 0, ImVec2(0, height))) {
			rSelectedLayer = layer;
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
			ImGui::Text(R.get<Cmp::Name>(layer).val.c_str());
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

void GUISystem::Render(entt::registry& R, const std::vector<entt::entity>& layersOrdered, entt::entity& rSelectedLayer, entt::entity drawingBoardEntity) {
	if (s_bShowGUI) {
		LayersWindow(R, layersOrdered, rSelectedLayer, drawingBoardEntity);
		//
		if (R.valid(rSelectedLayer)) {
			ImGui::Begin("Parameters");
			if (ImGui::GetIO().WantCaptureMouse && ImGui::IsWindowFocused())
				TNG::SetActiveHistoryToParameters(R, rSelectedLayer);
			bool bImGuiUsed = false;
			Cmp::Parameters& parameters = R.get<Cmp::Parameters>(rSelectedLayer);
			for (const auto& param : parameters.list) {
				bImGuiUsed |= param->ImGui(R, parameters.history, rSelectedLayer);
			}
			if (bImGuiUsed)
				TNG::MustRecomputeTexture(R, rSelectedLayer);
			ImGui::End();
		}
	}
}