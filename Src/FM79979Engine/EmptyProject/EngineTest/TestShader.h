#pragma once

class cTestShader :public cBaseShader
{
public:
	cTestShader(const char*e_strVS, const char*e_strPS, const wchar_t*e_strName = L"cTestShader");
	virtual ~cTestShader();
	static cTestShader*CreateShader(const char*e_strVSFileName, const char*e_strPSFileName, const WCHAR*e_strShaderName = L"cTestShader");
	virtual	void	Use(bool e_bUseLastWVPMatrix = true);
	virtual	void	Update(float e_fElpaseTime);
	static	const char*m_sVS;
	static	const char*m_sFS;
};