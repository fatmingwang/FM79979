#pragma once

#include "Glh.h"
#ifdef WIN32
#include "windows.h"
#elif defined(WASM)
#include <SDL2/SDL_video.h>
#endif
namespace FATMING_CORE
{
#define	OPENGL_ES_20	20
#define	OPENGL_ES_30	30
#define	OPENGL_ES_31	31
#define	OPENGL_ES_32	32
	class cOpenGLRender:public NamedTypedObject
	{
		void														CreateDefaultShader();
	public:
		bool														m_sMultisample;
#if defined(WIN32)
		HDC															m_Hdc;
		HANDLE														m_Handle;
		HGLRC														m_HGLRC;
		cOpenGLRender();
#elif defined(WASM)
		SDL_Window*													m_Handle;
#endif
		cOpenGLRender(Vector2 e_vGameResolution = Vector2(1920.f, 1080.f), Vector2 e_vViewportSize = Vector2(1920.f, 1080.f));
		virtual ~cOpenGLRender();
		DEFINE_TYPE_INFO();
		//while game resolution is change we want to re scale all data check this one
		Vector2														m_vGameScale;
		//viewport resolution or game resolution,both them could be same,but depend on the game type.
		Vector2														m_vGameResolution;
		//if you are using camera please set this too.
		Vector4														m_vViewPortSize;
		//in SDL2 it seems same as SDL_SetVideoMode
		Vector4														m_vDeviceViewPortSize;
		Vector4														m_vBGColor;
		eDeviceDirection											m_eDeviceDirection;
		//
		void														Update(float e_fElpaseTime);
		void														Render();
		POINT														ConvertCoordinate(int e_iPosX, int e_iPosY, POINT e_ViewPort);
		POINT														GetSDLMouseConvertCoordinate(int e_iPosX, int e_iPosY, POINT e_ViewPort);
		Vector2														GetViewPortAndGameResolutionScale();
		void														SetAcceptRationWithGameresolution(int e_iDeviceViewportWidth, int e_iDeviceViewportHeight, int e_iTargetResolutionWidth, int e_iTargetResolutionHeight);
		//
		static	bool												IsDoBatchRendering();
		//
		static  class cBatchDataMultiTexture*						m_spBatchDataMultiTexture;
		//
		static	std::vector<int>*									m_piSupportCompressedFormatVector;
		static  bool												m_sbSupportNonPowerOfTwoTexture;//old name g_bSupportNonPowerOfTwoTexture
		static	bool												m_sbVBOSupported;
		//because float problem...so type is int
		static	int													m_siOpenGLVersion;
		static	int													m_siOpenGL_ShaderVersion;
		//
		static	int													m_siOpenGLESVersion;
		static	int													m_siOpenGLES_ShaderVersion;
		//init shader
#if defined(WIN32) && !defined(UWP)
		void														Init(HWND e_Hwnd, bool e_bMultiSample = false);
#else
		void														Init();
#endif
		static	bool												IsCompressedFormatSupport(int e_iFormat);
		static void													DoBatchDataMultiTextureStart();
		static void													DoBatchDataMultiTextureEnd();
	};
}