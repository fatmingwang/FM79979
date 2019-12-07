#include "stdafx.h"
#include "Unzip.h"
#include "DownloadFile.h"
#include "DownloadContentPhase.h"

cDownloadContentPhase::cDownloadContentPhase(TiXmlElement*e_pPhasePopUpMessagerElement)
{
	m_pUnZip = 0;
	m_pDownloadFile = 0;
	m_pTextButton = 0;
	m_eStatus = eS_WAITNING_DOWNLOAD;
	if( e_pPhasePopUpMessagerElement )
	{
		m_pPhasePopUpMessager = new cPhasePopUpMessager(e_pPhasePopUpMessagerElement);
	}
	char*l_strDownloadFileName = "BluffingGirl/DownloadFont";
	m_strDownloadFontName = UT::CharToWchar(l_strDownloadFileName);
	cGlyphFontRender*l_pDownloadFont = new cGlyphFontRender(l_strDownloadFileName);
	cGameApp::m_spGlyphFontRenderVector->AddObject(l_pDownloadFont);
	//because name will strip folder name
	l_strDownloadFileName = "DownloadFont";
	m_strDownloadFontName = UT::CharToWchar(l_strDownloadFileName);
}

cDownloadContentPhase::~cDownloadContentPhase()
{
	SAFE_DELETE(m_pPhasePopUpMessager);
	SAFE_DELETE(m_pUnZip);
	SAFE_DELETE(m_pDownloadFile);
	SAFE_DELETE(m_pTextButton);
	cGameApp::m_spGlyphFontRenderVector->RemoveObject(m_strDownloadFontName.c_str());
}

bool	cDownloadContentPhase::StartToDownload(const char*e_strDownloadFileURL,const char*e_strDescription,const char* e_strTitle,const char*e_strDownloadFileName,const char*e_strZipPassword,const char*e_strExtractDirectory,bool e_bDeleteFileAfterExtract)
{
	SAFE_DELETE(m_pUnZip);
	SAFE_DELETE(m_pDownloadFile);
	m_eStatus = eS_WAITNING_DOWNLOAD;
	m_strExtractDirectory = e_strExtractDirectory;
	m_strZipPassword = e_strZipPassword;

	m_pPhasePopUpMessager->Init();

	m_pDownloadFile = new cDownloadFile();
	if(m_pDownloadFile->StartToDownload(e_strDownloadFileURL,e_strDescription, e_strTitle,e_strDownloadFileName))
		return true;
	return false;
}

void	cDownloadContentPhase::Update(float e_fElpaseTime)
{
	//10.168.1.133 / BluffingGirlWASM.html
#ifdef WASM
	m_bSatisfiedCondition = true;
#endif
	if( m_pPhasePopUpMessager && !m_pPhasePopUpMessager->IsSatisfiedCondition() )
	{
		m_pPhasePopUpMessager->Update(e_fElpaseTime);
		if(m_pPhasePopUpMessager->IsSatisfiedCondition())
		{
			if( m_pPhasePopUpMessager->GetCurrentWorkingObjectIndex() == 0 )
				m_bSatisfiedCondition = true;
		}
	}
	else
	if( m_pDownloadFile && !m_pTextButton )
	{
		if( !m_pUnZip )
		{
			m_pDownloadFile->Update(e_fElpaseTime);
			if(m_pDownloadFile->GetProgress() == -1 )
			{
				m_eStatus = eS_DOWNLOAD_FAILED;
#ifdef ENGLISH
				WCHAR	l_strDescription[] = {19979,36617,22833,25943,35531,30906,35496,32178,36335,36899,32218};
#else
				WCHAR	l_strDescription[] = L"Download failed!check connection status!";
#endif
				m_pTextButton = new cTextButton(Vector2(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2,cGameApp::m_spOpenGLRender->m_vGameResolution.y/2),cGameApp::GetGlyphFontRender(m_strDownloadFontName.c_str()),Vector4::HalfBright,l_strDescription);
			}
			else
			if(m_pDownloadFile->GetProgress() == 100)
			{
				m_eStatus = eS_UNZIPPING;
				m_pUnZip = new cUnZip();
				m_pUnZip->StartUnzipInDownloadFolder(m_pDownloadFile->m_strDownloadFileName.c_str(),this->m_strExtractDirectory.c_str(),this->m_strZipPassword.c_str());
			}
		}
		else
		{
			m_pUnZip->Update(e_fElpaseTime);
			if(m_pUnZip->GetProgress() == -1 )
			{
				m_eStatus = eS_UNZIP_FAILED;
#ifdef ENGLISH
				WCHAR	l_strDescription[] = {35299,22738,32302,22833,25943,35531,37325,26032,19979,36617};
#else
				WCHAR	l_strDescription[] = L"unzip failed!please download again!";
#endif
				m_pTextButton = new cTextButton(Vector2(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2,cGameApp::m_spOpenGLRender->m_vGameResolution.y/2),cGameApp::GetGlyphFontRender(m_strDownloadFontName.c_str()),Vector4::HalfBright,l_strDescription);
			}
			else
			if(m_pUnZip->GetProgress() == 100)
			{
				m_eStatus = eS_SHOW_ALL_DONE_MESSAGE;
#ifdef ENGLISH
				WCHAR	l_strDescription[] = L"Let's Play";
#else
				WCHAR	l_strDescription[] = {23433,L' ',35037,L' ',25104,L' ',21151};
#endif
				
				m_pTextButton = new cTextButton(Vector2(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2,cGameApp::m_spOpenGLRender->m_vGameResolution.y/2),cGameApp::GetGlyphFontRender(m_strDownloadFontName.c_str()),Vector4::HalfBright,l_strDescription);
			}		
		}
		if( m_pTextButton )
		{
			//m_pTextButton->SetScale(2.f);
		}
	}
	else
	if( m_pTextButton )
	{
		m_pTextButton->Update(e_fElpaseTime);
		this->m_bSatisfiedCondition = m_pTextButton->IsSatisfiedCondition();
	}
}

void	cDownloadContentPhase::Render()
{
	if( m_pPhasePopUpMessager && !m_pPhasePopUpMessager->IsSatisfiedCondition() )
	{
		m_pPhasePopUpMessager->Render();
	}
	else
	if( m_eStatus != eS_SHOW_ALL_DONE_MESSAGE &&
		m_eStatus != eS_DOWNLOAD_FAILED &&
		m_eStatus != eS_UNZIP_FAILED )
	{
		if( m_pUnZip )
		{
			m_pUnZip->Render();
		}
		else
		if( m_pDownloadFile )
		{
			m_pDownloadFile->Render();
		}
	}
	else
	if( m_pTextButton )
	{
		m_pTextButton->GetGlyphFontRender()->SetFontColor(Vector4(1,1,0,1));
		m_pTextButton->Render();
		m_pTextButton->GetGlyphFontRender()->SetFontColor(Vector4(1,1,1,1));
	}
}

void    cDownloadContentPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pPhasePopUpMessager && !m_pPhasePopUpMessager->IsSatisfiedCondition() )
	{
		m_pPhasePopUpMessager->MouseDown(e_iPosX,e_iPosY);
	}
	else
	if( m_pTextButton )
	{
		m_pTextButton->MouseDown(e_iPosX,e_iPosY);
	}
}

void    cDownloadContentPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pPhasePopUpMessager && !m_pPhasePopUpMessager->IsSatisfiedCondition() )
	{
		m_pPhasePopUpMessager->MouseMove(e_iPosX,e_iPosY);
	}
	else
	if( m_pTextButton )
	{
		m_pTextButton->MouseMove(e_iPosX,e_iPosY);
	}
}

void    cDownloadContentPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pPhasePopUpMessager && !m_pPhasePopUpMessager->IsSatisfiedCondition() )
	{
		m_pPhasePopUpMessager->MouseUp(e_iPosX,e_iPosY);
	}
	else
	if( m_pTextButton )
	{
		m_pTextButton->MouseUp(e_iPosX,e_iPosY);
	}
}

bool	cDownloadContentPhase::IsDownloadComplete()
{
	if( m_pTextButton )
		return m_pTextButton->IsSatisfiedCondition();
	return false;
}