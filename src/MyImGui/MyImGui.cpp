#include "MyImGui.hpp"

#include <imgui/imgui_internal.h>

ImGuiContext* MyImGui::m_context;

void MyImGui::BeginInstancesMenu() {
	ImGuiContext& g = GetContext();
	ImGuiViewport* viewport = g.Viewports[0];
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, 2 + viewport->Pos.y + g.NextWindowData.MenuBarOffsetMinVal.y + g.FontBaseSize + g.Style.FramePadding.y));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 28));
	ImGui::SetNextWindowViewport(viewport->ID); // Enforce viewport so we don't create our onw viewport when ImGuiConfigFlags_ViewportsNoMerge is set.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking
								  | ImGuiWindowFlags_NoTitleBar 
								  | ImGuiWindowFlags_NoResize 
								  | ImGuiWindowFlags_NoMove 
								  | ImGuiWindowFlags_NoScrollbar 
								  | ImGuiWindowFlags_NoSavedSettings
								  | ImGuiWindowFlags_NoBringToFrontOnFocus
	//							  | ImGuiWindowFlags_NoBackground
	;
	ImGui::Begin("##InstancesMenuBar", NULL, window_flags);
	ImGui::PopStyleVar(2);
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
}

void MyImGui::RightSideWindow() {
	float width = 260.0f;
	ImGuiContext& g = GetContext();
	ImGuiViewport* viewport = g.Viewports[0];
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + viewport->Size.x - width, 28 + 2 + viewport->Pos.y + g.NextWindowData.MenuBarOffsetMinVal.y + g.FontBaseSize + g.Style.FramePadding.y));
	ImGui::SetNextWindowSize(ImVec2(width, viewport->Size.y - 46));
	ImGui::SetNextWindowViewport(viewport->ID); // Enforce viewport so we don't create our onw viewport when ImGuiConfigFlags_ViewportsNoMerge is set.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
	ImGuiWindowFlags window_flags =     ImGuiWindowFlags_NoTitleBar
									  | ImGuiWindowFlags_NoResize
									  | ImGuiWindowFlags_NoMove
									  | ImGuiWindowFlags_NoScrollbar
									  //| ImGuiWindowFlags_NoSavedSettings
									  | ImGuiWindowFlags_NoBringToFrontOnFocus
		//							  | ImGuiWindowFlags_NoBackground
		;
	ImGui::Begin("##RightSideWindow", NULL, window_flags);
	ImGui::PopStyleVar(2);
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
	ImGui::End();
}