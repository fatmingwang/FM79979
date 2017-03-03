#include "../stdafx.h"
#include "ToneMapping.h"
#include "../Image/FrameBuffer.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
		static	const char*m_sVS;//vertical
		static	const char*m_sFS;
	cDownSampleBrightShader*cDownSampleBrightShader::CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,POINT e_DownSampleResolution,const WCHAR*e_strShaderName)
	{
		std::string l_strVS = UT::GetTxtFileContent(e_strVSFileName);
		std::string l_strPS = UT::GetTxtFileContent(e_strPSFileName);
		cDownSampleBrightShader*l_pShader = new cDownSampleBrightShader(l_strVS.c_str(),l_strPS.c_str(),e_DownSampleResolution,e_strShaderName);
		return l_pShader;
	}

	cDownSampleBrightShader::cDownSampleBrightShader(const char*e_strVS,const char*e_strPS,POINT e_DownSampleResolution,const WCHAR*e_strShaderName)
	:cBaseShader(e_strVS,e_strPS,e_strShaderName,true)
	{
		MyGlErrorTest();
		m_pFrameBuffer = new cFrameBuffer(e_DownSampleResolution.x,e_DownSampleResolution.y,false);
		m_uiPixelOffsetLocation = GetUniFormLocationByName("PixelOffset");
		m_vResolutionOffset.x = 1.f/e_DownSampleResolution.x;
		m_vResolutionOffset.y = 1.f/e_DownSampleResolution.y;
	}
	cDownSampleBrightShader::~cDownSampleBrightShader()
	{
		SAFE_DELETE(m_pFrameBuffer);
	}

	void	cDownSampleBrightShader::Use(bool e_bUseLastWVPMatrix)
	{
		cBaseShader::Use(e_bUseLastWVPMatrix);
		glUniform2f(m_uiPixelOffsetLocation,m_vResolutionOffset.x,m_vResolutionOffset.y);
	}

	cToneMappingShader*cToneMappingShader::CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,
		const char*e_strDownSampleVSFileName,const char*e_strDownSamplePSFileName,
		const char*e_strBlurHVSFileName,const char*e_strBlurHPSFileName,
		const char*e_strBlurVVSFileName,const char*e_strBlurVPSFileName,
		POINT e_DownSampleResolution,const WCHAR*e_strShaderName)
	{
		std::string l_strToneMappingVS = UT::GetTxtFileContent(e_strVSFileName);
		std::string l_strToneMappingPS = UT::GetTxtFileContent(e_strPSFileName);
		cToneMappingShader*l_pShader = new cToneMappingShader(l_strToneMappingVS.c_str(),l_strToneMappingPS.c_str(),
			e_strDownSampleVSFileName,e_strDownSamplePSFileName,
			e_strBlurHVSFileName,e_strBlurHPSFileName,
			e_strBlurVVSFileName,e_strBlurVPSFileName,
			e_DownSampleResolution,e_strShaderName);
		return l_pShader;
	}

	cToneMappingShader::cToneMappingShader(const char*e_strVS,const char*e_strPS,
			const char*e_strDownSampleVSFileName,const char*e_strDownSamplePSFileName,
			const char*e_strBlurHVSFileName,const char*e_strBlurHPSFileName,
			const char*e_strBlurVVSFileName,const char*e_strBlurVPSFileName,
		POINT e_DownSampleResolution,const WCHAR*e_strShaderName)
	:cBaseShader(e_strVS,e_strPS,e_strShaderName,true)
	{
		MyGlErrorTest();
		m_pHoricontalBlur = nullptr;
		m_pVerticalBlur = nullptr;
		m_pDownSampleBrightShader = cDownSampleBrightShader::CreateShader(e_strDownSampleVSFileName,e_strDownSamplePSFileName,e_DownSampleResolution);
		m_pOriginalFrameBuffer = new cFrameBuffer((int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y);
		m_pHoricontalBlur = cBlurShader::CreateHorizontalBlurShader(e_strBlurHVSFileName,e_strBlurHPSFileName,e_DownSampleResolution);
		m_pVerticalBlur = cBlurShader::CreateVerticalBlurShader(e_strBlurHVSFileName,e_strBlurVPSFileName,e_DownSampleResolution);
		m_uiTexLoacation = GetUniFormLocationByName("tFull");
		m_uiBlurXTexLoacation = GetUniFormLocationByName("tBlur");
		m_uiExposureLevelLoacation = GetUniFormLocationByName("fExposureLevel");
		m_fExposureLevel = 1.5;
	}

	cToneMappingShader::~cToneMappingShader()
	{
		SAFE_DELETE(m_pHoricontalBlur);
		SAFE_DELETE(m_pVerticalBlur);
		SAFE_DELETE(m_pDownSampleBrightShader);
		SAFE_DELETE(m_pOriginalFrameBuffer);
	}

	void	cToneMappingShader::Use(bool e_bUseLastWVPMatrix)
	{
		cBaseShader::Use(e_bUseLastWVPMatrix);
		if(m_pVerticalBlur)
		{
			glUniform1i(m_uiTexLoacation, 0);
			glActiveTexture(GL_TEXTURE0); 
			glBindTexture(GL_TEXTURE_2D, m_pOriginalFrameBuffer->GetTextureID());
			glUniform1i(m_uiBlurXTexLoacation, 1);
			glActiveTexture(GL_TEXTURE1); 
			glBindTexture(GL_TEXTURE_2D,this->m_pVerticalBlur->GetFrameBuffer()->GetTextureID());
			//
			glUniform1f(m_uiExposureLevelLoacation, this->m_fExposureLevel);
			
		}
	}

	void	cToneMappingShader::StartDraw()
	{
		if(this->m_pOriginalFrameBuffer)
		{
			m_pOriginalFrameBuffer->StartDraw();
		}
	}

	void	cToneMappingShader::DoDownSample()
	{
		this->m_pDownSampleBrightShader->GetFrameBuffer()->StartDraw();
		this->m_pDownSampleBrightShader->Use();
		POINT l_Pos = { 0,0};
		POINT l_Size = { (int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y };
		cTexture::ApplyImage(this->m_pOriginalFrameBuffer->GetTextureID());
		m_pOriginalFrameBuffer->DrawBuffer(l_Pos,l_Size,this->m_pDownSampleBrightShader->GetName());
		m_pDownSampleBrightShader->GetFrameBuffer()->EndDraw();
	}

	void	cToneMappingShader::DoBlur(cBlurShader*e_pBlurShader,cFrameBuffer*e_pFrameBuffer)
	{
		if(e_pBlurShader)
		{
			e_pBlurShader->GetFrameBuffer()->StartDraw();
			e_pBlurShader->Use();
			POINT l_Pos = { 0,0};
			POINT l_Size = { (int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y };
			e_pFrameBuffer->DrawBuffer(l_Pos,l_Size,e_pBlurShader->GetName());
			e_pBlurShader->GetFrameBuffer()->EndDraw();
		}
	}

	void	cToneMappingShader::EndDraw()
	{
		if(m_pOriginalFrameBuffer)
		{
			m_pOriginalFrameBuffer->EndDraw();
			DoDownSample();
			DoBlur(this->m_pHoricontalBlur,m_pDownSampleBrightShader->GetFrameBuffer());
			DoBlur(this->m_pVerticalBlur,m_pHoricontalBlur->GetFrameBuffer());
			this->Use();
			float	l_fTextureCoordinate[] ={0,1,1,0};
			DrawQuadWithTextureAndColorAndCoordinate(0.f,0.f,0.f,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,Vector4::One,l_fTextureCoordinate,Vector3::Zero,this->GetName());
			//if( cGameApp::m_sucKeyData['D'] )
			{
				DebugRender();
			}
		}	
	}

	void	cToneMappingShader::DebugRender()
	{
		UseShaderProgram(DEFAULT_SHADER);
		if( cGameApp::m_sucKeyData['D'] )
		{
			POINT l_Size ={ (int)cGameApp::m_svGameResolution.x/2,(int)cGameApp::m_svGameResolution.y/2 };
			POINT l_Pos = { 0,0};
			this->m_pOriginalFrameBuffer->DrawBuffer(l_Pos,l_Size);
			cGameApp::RenderFont((int)l_Pos.x,(int)l_Pos.y,L"Full");

			l_Pos.y += (int)cGameApp::m_svGameResolution.y/2;
			this->m_pDownSampleBrightShader->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
			cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"DownSample");
			l_Pos.x  = (int)cGameApp::m_svGameResolution.x/2;
			l_Pos.y  = 0;
			this->m_pHoricontalBlur->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
			cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"HBlur");
			l_Pos.y += (int)cGameApp::m_svGameResolution.y/2;
			this->m_pVerticalBlur->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
			cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"VBlur");
		}
		else
		{
			POINT l_Size ={ (int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y };
			POINT l_Pos = { 0,0};
			if( cGameApp::m_sucKeyData['1'] )
			{
				this->m_pOriginalFrameBuffer->DrawBuffer(l_Pos,l_Size);
				cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"Full");
			}
			else
			if( cGameApp::m_sucKeyData['2'] )
			{
				this->m_pDownSampleBrightShader->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
				cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"DownSample");
			}
			else
			if( cGameApp::m_sucKeyData['3'] )
			{
				this->m_pHoricontalBlur->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
				cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"HBlur");
			}
			else
			if( cGameApp::m_sucKeyData['4'] )
			{
				this->m_pVerticalBlur->GetFrameBuffer()->DrawBuffer(l_Pos,l_Size);
				cGameApp::RenderFont(l_Pos.x,l_Pos.y,L"VBlur");
			}
		}
	}

}