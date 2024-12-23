#pragma once
#include <vector>
#include <functional>
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


void		SetImGuiGetCameraCursorPosition(std::function<void(long&, long&)> e_Function, int e_iContextIndex = 0);
void		SetImGuiCameraPositionConvertFunction(std::function<float* (float*)> e_Function, int e_iContextIndex = 0);
void		SetImGuiMouseEnable(bool e_bMouseEventEnable, int e_iContextIndex = 0);
void		EditboxFocusChangedChangeMouseEnable(bool e_bMouseEventEnable);
void		AllImGuiMouseEnable();

void		ImGui_ImplOpenGL3_Shutdown();
void		ImGui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data,float*e_pCameraMatrix = nullptr);
void		ImGui_StartFrame(float*e_pGameResolutionSize = nullptr,int e_iContextIndex = 0);
void		ImGui_EndFrame(float*e_pfMatrix = nullptr, float* e_pGameResolutionSize = nullptr);
#ifdef WIN32
bool		ImGui_ImplOpenGL3_Init(void* hwnd,const char* glsl_version = nullptr,int e_iNumContext = 1);
#elif defined(WASM)
#ifdef USE_SDL2
bool		ImGui_ImplSDL2_Init(struct SDL_Window* window);
#else
bool		ImGui_ImplSDL2_Init();
#endif
bool		ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
#else
bool		ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
#endif

struct ImFont*	GetImFont(const char* e_strFileNme);
struct ImFont*	GetFontByFileName(const std::string& fontFileName);
void			AddImGuiFonts(const std::vector<const char*>& fontPaths, const std::vector<float> e_FontSizeVector);