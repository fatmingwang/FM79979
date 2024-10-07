#pragma once
#ifdef WASM
#include<SDL2/SDL_events.h>
#endif

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data);
#ifdef WIN32
bool     ImGui_ImplWin32_Init(void* hwnd);
bool     ImGui_ImplWin32_InitForOpenGL(void* hwnd);
void     ImGui_ImplWin32_Shutdown();
void     ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
bool	ImGui_ImplSDL2_Init(struct SDL_Window* window);
bool	ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
void	ImGui_ImplSDL2_NewFrame();
#endif