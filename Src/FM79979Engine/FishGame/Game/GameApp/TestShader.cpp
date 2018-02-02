#include "stdafx.h"
//#include "stdafx.h"
#include "TestShader.h"
namespace FATMING_CORE
{
	extern cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList;
}
const char*cTestShader::m_sVS = "";
const char*cTestShader::m_sFS = "";


cTestShader::cTestShader(const char*e_strVS, const char*e_strPS, const wchar_t*e_strName)
:cBaseShader(e_strVS, e_strPS, e_strName, true)
{
	g_pAll2DShaderList->AddObjectNeglectExist(this);
}

cTestShader::~cTestShader()
{
	g_pAll2DShaderList->RemoveObjectWithoutDelete(this);
}

cTestShader*cTestShader::CreateShader(const char*e_strVSFileName, const char*e_strPSFileName, const WCHAR*e_strShaderName)
{
	std::string l_strVS = UT::GetTxtFileContent(e_strVSFileName);
	std::string l_strPS = UT::GetTxtFileContent(e_strPSFileName);
	cTestShader*l_pShader = new cTestShader(l_strVS.c_str(), l_strPS.c_str(), e_strShaderName);
	return l_pShader;
}

void	cTestShader::Use(bool e_bUseLastWVPMatrix)
{
	cBaseShader::Use(e_bUseLastWVPMatrix);
}

void	cTestShader::Update(float e_fElpaseTime)
{

}