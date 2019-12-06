#pragma once

#include "Shader.h"

namespace FATMING_CORE
{
	class	cFrameBuffer;
	class	cToneMappingShader;
	class	cBlurShader:public cBaseShader
	{
		friend class	cToneMappingShader;
		cFrameBuffer*	m_pFrameBuffer;
		GLuint			m_uiResolutionLocation;
		GLuint			m_uiPixelOffsetLocation;
		Vector2			m_vResolutionOffset;
	public:
		static cBlurShader*	CreateHorizontalBlurShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_Resolution);
		static cBlurShader*	CreateVerticalBlurShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_Resolution);
		cBlurShader(const char*e_strVS,const char*e_strPS,POINT e_Resolution,const wchar_t*e_strName = L"clBlurShader");
		virtual ~cBlurShader();
		cFrameBuffer*	GetFrameBuffer(){return m_pFrameBuffer;}
		virtual	void	Use(bool e_bUseLastWVPMatrix = true);
		//
		static	const char*m_sVSH;//horizontal
		static	const char*m_sVSV;//vertical
		static	const char*m_sFS;
	};

}