#pragma once

class MyImGui {
public:
	static void BeginInstancesMenu();
	static void RightSideWindow();

	static inline ImGuiContext& GetContext() { return *m_context; }
	static inline void CreateContext() { m_context = ImGui::CreateContext(); }
private:
	static ImGuiContext* m_context;
};