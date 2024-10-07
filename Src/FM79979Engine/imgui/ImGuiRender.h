#pragma once


// Follow "Getting Started" link and check examples/ folder to learn about using backends!
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data);
#ifdef WIN32
bool     ImGui_ImplWin32_Init(void* hwnd);
bool     ImGui_ImplWin32_InitForOpenGL(void* hwnd);
void     ImGui_ImplWin32_Shutdown();
void     ImGui_ImplWin32_NewFrame();
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif