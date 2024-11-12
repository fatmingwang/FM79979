#pragma once
#include <vector>
#include <string>
#ifdef WASM
//#define USE_SDL2
#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_mouse.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif
#endif

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data);
void     ImGui_StartFrame();
void     ImGui_EndFrame();
#ifdef WIN32
bool     ImGui_ImplWin32_Init(void* hwnd);
bool     ImGui_ImplWin32_InitForOpenGL(void* hwnd);
void     ImGui_ImplWin32_Shutdown();
//please call ImGui_Impl_NewFrame
void     ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
#ifdef USE_SDL2
bool	ImGui_ImplSDL2_Init(struct SDL_Window* window);
#else
bool	ImGui_ImplSDL2_Init();
#endif
bool	ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
//please call ImGui_Impl_NewFrame
void	ImGui_ImplSDL2_NewFrame();
#endif

struct ImFont* GetImFont(const char* e_strFileNme);
void AddImGuiFonts(const std::vector<const char*>& fontPaths, const std::vector<float> e_FontSizeVector);
struct ImFont* GetFontByFileName(const std::string& fontFileName);