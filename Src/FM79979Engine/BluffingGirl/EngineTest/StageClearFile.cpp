#include "stdafx.h"
#include "StageClearFile.h"

cNamedTypedObjectVector<cStageClearFile::cStageClearCheck>*	cStageClearFile::m_pStageClearCheckVector = 0;

cStageClearFile::cStageClearFile()
{
	m_pStageClearCheckVector = new cNamedTypedObjectVector<cStageClearFile::cStageClearCheck>;
}
cStageClearFile::~cStageClearFile()
{
	SAFE_DELETE(m_pStageClearCheckVector);
}

int		GetMagicID(const WCHAR*e_strStageName)
{
	if( !e_strStageName )
		return -1;
	bool	l_bFileCorrect = false;
	int	l_iLength = wcslen(e_strStageName);
	int	l_iMagicID = 0;
	for( int i=0;i<l_iLength;++i )
	{
		l_iMagicID += e_strStageName[i];
	}
#ifdef ANDROID
	std::wstring l_str2 = GetDeviceID();
	for(int i=0;i<(int)l_str2.length();++i)
	{
		l_iMagicID += l_str2[i];
	}
#endif
	return l_iMagicID;
}

bool	cStageClearFile::IsStageClear(const WCHAR*e_strStageName)
{
	cStageClearCheck*l_pStageClearCheck = 0;
	l_pStageClearCheck = m_pStageClearCheckVector->GetObject(e_strStageName);
	if( !l_pStageClearCheck )
	{
		l_pStageClearCheck = new cStageClearCheck();
		l_pStageClearCheck->SetName(e_strStageName);
		m_pStageClearCheckVector->AddObjectNeglectExist(l_pStageClearCheck);
	}
	else//already wrote
		return l_pStageClearCheck->m_bStageCleard;
	int	l_iMagicID = GetMagicID(e_strStageName);
	if( l_iMagicID == -1 )
		return false;
	bool	l_bFileCorrect = false;
	cSkipHeaderBinaryFile*l_pFile = new cSkipHeaderBinaryFile(l_iMagicID);
	std::string	l_strFileName = UT::WcharToChar(e_strStageName);
	l_strFileName += ".xml";
	l_pFile->Openfile(l_strFileName.c_str());
	void*l_pData = l_pFile->GetDataFile(0);
	if( l_pData )
	{
		l_bFileCorrect = true;
		l_pStageClearCheck->m_bStageCleard = true;
	}
	else
		l_pStageClearCheck->m_bStageCleard = false;
	SAFE_DELETE(l_pFile);
	return l_bFileCorrect;
}

bool	cStageClearFile::StageClear(const WCHAR*e_strStageName)
{
	cStageClearCheck*l_pStageClearCheck = 0;
	l_pStageClearCheck = m_pStageClearCheckVector->GetObject(e_strStageName);
	if( !l_pStageClearCheck )
	{
		l_pStageClearCheck = new cStageClearCheck();
		l_pStageClearCheck->m_bStageCleard = true;
		l_pStageClearCheck->SetName(e_strStageName);
		m_pStageClearCheckVector->AddObjectNeglectExist(l_pStageClearCheck);
	}
	else//already wrote
	if( l_pStageClearCheck->m_bStageCleard )
	{
		return true;
	}
	int	l_iMagicID = GetMagicID(e_strStageName);
	if( l_iMagicID == -1 )
		return false;
	bool	l_bFileCorrect = false;
	cSkipHeaderBinaryFile*l_pFile = new cSkipHeaderBinaryFile(l_iMagicID);
	std::string	l_strFileName = UT::WcharToChar(e_strStageName);
	l_strFileName += ".xml";
	l_pFile->Writefile(l_strFileName.c_str(),true,true);
	l_pFile->AddGarbageEndFile();
	l_pFile->CloseFile();
	SAFE_DELETE(l_pFile);
	l_pStageClearCheck->m_bStageCleard = true;
	return true;
}


bool	cStageClearFile::IsGirlUnlock(const WCHAR*e_strStageName)
{
	return IsStageClear(e_strStageName);
}

bool	cStageClearFile::GirlUnlock(const WCHAR*e_strStageName)
{
	return StageClear(e_strStageName);
}