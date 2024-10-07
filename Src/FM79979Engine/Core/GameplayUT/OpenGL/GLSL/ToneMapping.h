#pragma once

#include "Shader.h"
#include "Blur.h"

namespace FATMING_CORE
{
	class	cFrameBuffer;

	class	cDownSampleBrightShader:public cBaseShader
	{
		cFrameBuffer*	m_pFrameBuffer;
		GLuint			m_uiPixelOffsetLocation;
		Vector2			m_vResolutionOffset;
	public:
		cDownSampleBrightShader(const char*e_strVS,const char*e_strPS,POINT e_DownSampleResolution,const WCHAR*e_strShaderName);
		virtual ~cDownSampleBrightShader();
		static cDownSampleBrightShader*CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_DownSampleResolution,const WCHAR*e_strShaderName = L"cDownSampleBrightShader");
		virtual	void	Use(bool e_bUseLastWVPMatrix = true);
		cFrameBuffer*	GetFrameBuffer(){return m_pFrameBuffer;}
		static	const char*m_sVS;//vertical
		static	const char*m_sFS;
	};
	class	cToneMappingShader:public cBaseShader
	{
		cFrameBuffer*	m_pOriginalFrameBuffer;
		cDownSampleBrightShader*m_pDownSampleBrightShader;
		cBlurShader*m_pHoricontalBlur;
		cBlurShader*m_pVerticalBlur;

		GLuint	m_uiBlurXTexLoacation;

		GLuint	m_uiExposureLevelLoacation;
		float	m_fExposureLevel;
		//GLuint	m_uiBlurYTexLoacation;
		void	DoBlur(cBlurShader*e_pBlurShader,cFrameBuffer*e_pFrameBuffer);
		void	DoDownSample();
	public:
		cToneMappingShader(const char*e_strVS,const char*e_strPS,
			const char*e_strDownSampleVSFileName,const char*e_strDownSamplePSFileName,
			const char*e_strBlurHVSFileName,const char*e_strBlurHPSFileName,
			const char*e_strBlurVVSFileName,const char*e_strBlurVPSFileName,
			POINT e_DownSampleResolution,const WCHAR*e_strShaderName);
		static cToneMappingShader*CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,
			const char*e_strDownSampleVSFileName,const char*e_strDownSamplePSFileName,
			const char*e_strBlurHVSFileName,const char*e_strBlurHPSFileName,
			const char*e_strBlurVVSFileName,const char*e_strBlurVPSFileName,
			POINT e_DownSampleResolution,const WCHAR*e_strShaderName = L"cToneMappingShader");
		~cToneMappingShader();
		virtual	void	Use(bool e_bUseLastWVPMatrix = true);
		void			StartDraw();
		void			EndDraw();
		void			DebugRender();
		static	const char*m_sVS;//vertical
		static	const char*m_sFS;
	};
}