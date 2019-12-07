#pragma once

#include "Glh.h"
#ifdef WIN32
#include "windows.h"
#endif
namespace FATMING_CORE
{
	class cOpenGLRender
	{
	public:
		bool														m_sMultisample;
#if defined(WIN32)
		HDC															m_Hdc;
		HANDLE														m_Handle;
		HGLRC														m_HGLRC;
		cOpenGLRender();
		cOpenGLRender(Vector2 e_vGameResolution = Vector2(1920.f, 1080.f), Vector2 e_vViewportSize = Vector2(1920.f, 1080.f));
#else
		cOpenGLRender(Vector2 e_vGameResolution, Vector2 e_vViewportSize);
#endif
		~cOpenGLRender();
		//while game resolution is change we want to re scale all data check this one
		Vector2														m_vGameScale;
		//viewport resolution or game resolution,both them could be same,but depend on the game type.
		Vector2														m_vGameResolution;
		Vector4														m_vViewPortSize;
		Vector4														m_vDeviceViewPortSize;
		Vector4														m_vBGColor;
		eDeviceDirection											m_eDeviceDirection;
		//
		void														Update(float e_fElpaseTime);
		void														Render();
		POINT														ConvertCoordinate(int e_iPosX, int e_iPosY, POINT e_ViewPort);
		Vector2														GetViewPortAndGameResolutionScale();
		void														SetAcceptRationWithGameresolution(int e_iDeviceViewportWidth, int e_iDeviceViewportHeight, int e_iTargetResolutionWidth, int e_iTargetResolutionHeight);
		//
		static	std::vector<int>*									m_piSupportCompressedFormatVector;
		static	float												m_sfOpenGLVersion;
		static  bool												m_sbSupportNonPowerOfTwoTexture;//old name g_bSupportNonPowerOfTwoTexture
		//init shader
#ifdef WIN32
		void														Init(HWND e_Hwnd, bool e_bMultiSample = false);
#endif
		static	bool												IsCompressedFormatSupport(int e_iFormat);
	};
}