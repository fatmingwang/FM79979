#include "stdafx.h"
#include "GameDefine.h"

extern cLanguageFile*g_pLanguageFile;
const wchar_t*g_strFishGameFileName[eFGFN_MAX] =
{
	L"MESSAGE_MPDILIST",
	L"Language/Language.xml",
	L"SceneManager.xml",
};


std::string	GetFishGameFileName(eFishGameFileName e_eFishGameFileName)
{
	std::string l_strFileName;
	const wchar_t*l_strFileKey = g_strFishGameFileName[e_eFishGameFileName];
	if (g_pLanguageFile)
	{
		auto l_pstrFileName = g_pLanguageFile->GetFileName(l_strFileKey);
		if (l_pstrFileName)
		{
			l_strFileName = UT::WcharToChar(l_pstrFileName);
		}
		else
		{
			l_strFileName = UT::WcharToChar(l_strFileKey);
		}
	}
	else
	{
		l_strFileName = UT::WcharToChar(l_strFileKey);
	}
	return l_strFileName;
}

std::wstring	GetFishGameFileNameW(eFishGameFileName e_eFishGameFileName)
{
	std::wstring l_strFileName;
	const wchar_t*l_strFileKey = g_strFishGameFileName[e_eFishGameFileName];
	if (g_pLanguageFile)
	{
		auto l_pstrFileName = g_pLanguageFile->GetFileName(l_strFileKey);
		if (l_pstrFileName)
		{
			l_strFileName = l_pstrFileName;
		}
		else
		{
			l_strFileName = l_strFileKey;
		}
	}
	else
	{
		l_strFileName = l_strFileKey;
	}
	return l_strFileName;
}