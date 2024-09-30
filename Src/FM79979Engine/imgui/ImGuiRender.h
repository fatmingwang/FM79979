#pragma once


bool	ImGuiInit();
bool	ImGuiShutdown();
bool	ImGuiUpdate();
bool	ImGuiRender();

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data);