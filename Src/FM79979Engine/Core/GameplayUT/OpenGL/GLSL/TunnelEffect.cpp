#include "TunnelEffect.h"
namespace FATMING_CORE
{

	const char*cTunnelEffect::m_sVS = "";//vertical
	const char*cTunnelEffect::m_sFS = "";
	extern cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList;
	cTunnelEffect*cTunnelEffect::CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,const WCHAR*e_strShaderName )
	{
		std::string l_strVS = UT::GetTxtFileContent(e_strVSFileName);
		std::string l_strPS = UT::GetTxtFileContent(e_strPSFileName);
		cTunnelEffect*l_pShader = new cTunnelEffect(l_strVS.c_str(),l_strPS.c_str(),e_strShaderName);
		return l_pShader;
	}

	cTunnelEffect::cTunnelEffect(const char*e_strVS,const char*e_strPS,const wchar_t*e_strName)
	:cBaseShader(e_strVS,e_strPS,e_strName,true)
	{
		m_uTimeToAnimLocation = GetUniFormLocationByName("time");
		m_fTimeToAnim = 90.f;
		g_pAll2DShaderList->AddObjectNeglectExist(this);
	}

	cTunnelEffect::~cTunnelEffect()
	{
		g_pAll2DShaderList->RemoveObjectWithoutDelete(this);
	}

	void	cTunnelEffect::Use(bool e_bUseLastWVPMatrix)
	{
		cBaseShader::Use(e_bUseLastWVPMatrix);
		glUniform1f(m_uTimeToAnimLocation,m_fTimeToAnim);
	}
	void	cTunnelEffect::Update(float e_fElpaseTime)
	{
		m_fTimeToAnim += e_fElpaseTime;
		//if( m_fTimeToAnim >= 270.f )
		{
			//m_fTimeToAnim = 90.f;
		}
		
	}

	void	cTunnelEffect::SetTime(float e_fTime)
	{
		glUniform1f(m_uTimeToAnimLocation,e_fTime);	
	}
}