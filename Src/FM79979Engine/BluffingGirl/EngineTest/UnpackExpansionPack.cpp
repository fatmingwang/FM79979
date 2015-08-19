#include "stdafx.h"
#include "UnpackExpansionPack.h"
#include "Unzip.h"

cUnpackExpansionPack::cUnpackExpansionPack()
{
	m_bCheckedFileExists = UT::IsFileExists(m_strCheckedFileName.c_str());
#ifdef ENGLISH
	m_strUnzipFileName = "/Android/obb/com.wh.BluffingGirlEN/main.2.com.wh.BluffingGirl.obb";
#else
	m_strUnzipFileName = "/Android/obb/com.wh.BluffingGirl/main.2.com.wh.BluffingGirl.obb";
#endif
	m_strCheckedFileName = "ExpansionpackFile.bin";
	m_pUnZip = 0;
	cNodeISAX	l_cNodeISAX;
	if(l_cNodeISAX.ParseDataIntoXMLNode("PhaseData/Unpack.xml"))
	{
		TiXmlElement*l_pRoot = l_cNodeISAX.GetRootElement();
		const WCHAR*l_strExpansionPackFileName = l_pRoot->Attribute(L"ExpansionPackFileName");
		m_strUnzipFileName = UT::WcharToChar(l_strExpansionPackFileName);
	}
}

cUnpackExpansionPack::~cUnpackExpansionPack()
{
	SAFE_DELETE(m_pUnZip);
}


bool	cUnpackExpansionPack::IsUnpacked()
{
	return false;
}

void	cUnpackExpansionPack::DoUnpack()
{

}

bool	cUnpackExpansionPack::IsInUnpackProgress()
{
#if defined(ANDROID)
	if( !m_pUnZip )
	{
		if(m_bCheckedFileExists)
		{
			return false;
		}
		m_pUnZip = new cUnZip();
		m_pUnZip->StartUnzipInSystemFolder(m_strUnzipFileName.c_str(),"Video");
		return false;
	}
	return m_pUnZip->GetProgress() == 100 || m_pUnZip->GetProgress() == -1 ? false:true;
	return false;
#endif
	return false;
}

void	cUnpackExpansionPack::Update(float e_fElpaseTime)
{
	if( m_pUnZip )
	{
		m_pUnZip->Update(e_fElpaseTime);
		if( m_pUnZip->GetProgress() == 100 )
		{
			UT::MyFileOpen(m_strCheckedFileName.c_str(),"w");
			SAFE_DELETE(m_pUnZip);
			m_bCheckedFileExists = true;
		}
	}
}

void	cUnpackExpansionPack::Render()
{
	if( !m_pUnZip || m_pUnZip->GetProgress() == 0 )
		return;
	WCHAR l_str1[] = {21021,27425,22519,34892,36039,28304,20998,26512,20013,0};
	std::wstring	l_str = l_str1;//L"extract file progress:";
	l_str += ValueToStringW(m_pUnZip->GetProgress());

	
#ifdef ENGLISH
	cGameApp::RenderFont(cGameApp::m_svGameResolution.x/2-300,cGameApp::m_svGameResolution.y/2+100,L"resource initialize(only for first time)");
#else
	cGameApp::RenderFont(cGameApp::m_svGameResolution.x/2-300,cGameApp::m_svGameResolution.y/2,l_str);
#endif
}