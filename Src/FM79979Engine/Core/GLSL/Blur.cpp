#include "../stdafx.h"
#include "Blur.h"
#include "../Image/FrameBuffer.h"


namespace FATMING_CORE
{
	//extern cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList;
		//
	const char*cBlurShader::m_sVSH = "";
	const char*cBlurShader::m_sVSV = "";
	const char*cBlurShader::m_sFS = "";
	cBlurShader*	cBlurShader::CreateHorizontalBlurShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_Resolution)
	{
		std::string l_strVS = UT::GetTxtFileContent(e_strVSFileName);
		std::string l_strPS = UT::GetTxtFileContent(e_strPSFileName);
		cBlurShader*l_pBlurShader = new cBlurShader(l_strVS.c_str(),l_strPS.c_str(),e_Resolution,L"HorizontalBlur");
		l_pBlurShader->m_vResolutionOffset.y = 0;
		return l_pBlurShader;
	}

	cBlurShader*	cBlurShader::CreateVerticalBlurShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_Resolution)
	{
		std::string l_strVS = UT::GetTxtFileContent(e_strVSFileName);
		std::string l_strPS = UT::GetTxtFileContent(e_strPSFileName);
		cBlurShader*l_pBlurShader = new cBlurShader(l_strVS.c_str(),l_strPS.c_str(),e_Resolution,L"VerticalBlur");
		l_pBlurShader->m_vResolutionOffset.x = 0;
		return l_pBlurShader;
	}
	cBlurShader::cBlurShader(const char*e_strVS,const char*e_strPS,POINT e_Resolution, const wchar_t*e_strName)
	:cBaseShader(e_strVS,e_strPS,e_strName,true)
	{
		m_pFrameBuffer = new cFrameBuffer(e_Resolution.x,e_Resolution.y);
		m_uiResolutionLocation = GetUniFormLocationByName("BlurOffset");
		m_uiPixelOffsetLocation = GetUniFormLocationByName("PixelOffset");
		m_vResolutionOffset.x = 1.f/e_Resolution.x;
		m_vResolutionOffset.y = 1.f/e_Resolution.y;
		//g_pAll2DShaderList->AddObjectNeglectExist(this);
	}
	cBlurShader::~cBlurShader()
	{
		SAFE_DELETE(m_pFrameBuffer);
		//g_pAll2DShaderList->RemoveObjectWithoutDelete(this->GetName());
	}

	void	cBlurShader::Use(bool e_bUseLastWVPMatrix)
	{
		cBaseShader::Use(e_bUseLastWVPMatrix);
		glUniform2f(m_uiResolutionLocation,m_vResolutionOffset.x,m_vResolutionOffset.y);
		glUniform2f(m_uiPixelOffsetLocation,m_vResolutionOffset.x,m_vResolutionOffset.y);
	}
}