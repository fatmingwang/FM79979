#include "stdafx.h"
#include "DownloadContentPhase.h"
#include "ChoiceGirlPhase.h"
#include "StageClearFile.h"
#include "BluffingGirlApp.h"
//<ChoiceGirlPhase PI="Image/Girl/ChoiceGirl.pi">
//	<GirlData Name="1" Image="1"/>
//	<GirlData Name="2" Image="2"/>
//	<GirlData Name="3" Image="3"/>
//	<GirlData Name="4" Image="4"/>
//	<GirlData Name="5" Image="5"/>
//</ChoiceGirlPhase>
cChoiceGirlPhase::cChoiceGirlPhase(const char*e_strFileName)
{
	this->SetName(CHOICE_GIRL_PHASE_NAME);
	m_pCheckFileXmlElement = 0;
	m_pDownloadPopupMessageXmlElement = 0;
	m_pDownloadContentPhase = 0;
	m_pBGMPDI = 0;
	this->m_strFileName = e_strFileName;
	Vector4	l_vCollideRange(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	m_pScroller = 0;
	m_strNextPhaseName = CHOICE_GIRL_STAGE_PHASE_NAME;
	//m_pBackButton = new cImageButton(l_pCommonMPDIList->GetObject(L"Back"));
}

cChoiceGirlPhase::~cChoiceGirlPhase()
{
	Destroy();
}

void	cChoiceGirlPhase::Destroy()
{
	m_pBGMPDI = 0;
	m_MainfunctionButton.Destroy();
	m_GirlUnlokImageVector.Destroy();
	SAFE_DELETE(m_pCheckFileXmlElement);
	SAFE_DELETE(m_pDownloadPopupMessageXmlElement);
	SAFE_DELETE(m_pDownloadContentPhase);
	SAFE_DELETE(m_pScroller);
	m_ResourceStamp.ReleaseNewResource();
}

bool	cChoiceGirlPhase::DoCheckVideoFile()
{
	if(!m_pCheckFileXmlElement)
		return true;
	TiXmlElement*l_pElement = GetXmlElementByNameFromElement(this->CurrentSelectGirlName(),m_pCheckFileXmlElement);
	if(!l_pElement)
		return true;
	const WCHAR*l_strCheckFileName = l_pElement->Attribute(L"CheckFileName");
	std::string	l_strVideoName = ValueToString(l_strCheckFileName);
	if( !UT::IsFileExists(l_strVideoName.c_str()) )
	{
#ifdef WIN32
		assert("video is not exists!");
		return true;
#endif
		m_pDownloadContentPhase = new cDownloadContentPhase(m_pDownloadPopupMessageXmlElement);
		const WCHAR*l_strDownloadURL = l_pElement->Attribute(L"DownloadURL");
		const WCHAR*l_strDescription = l_pElement->Attribute(L"Description");
		const WCHAR*l_strTitle = l_pElement->Attribute(L"Title");
		const WCHAR*l_strSaveName = l_pElement->Attribute(L"SaveName");
		const WCHAR*l_strDeleteDownloadFile = l_pElement->Attribute(L"DeleteDownloadFile");
		const WCHAR*l_strExtractDirectory = l_pElement->Attribute(L"ExtractDirectory");
		const WCHAR*l_strPassword = l_pElement->Attribute(L"Password");
		m_pDownloadContentPhase->StartToDownload(	ValueToString(l_strDownloadURL).c_str(),
													ValueToString(l_strDescription).c_str(),
													ValueToString(l_strTitle).c_str(),
													ValueToString(l_strSaveName).c_str(),
													ValueToString(l_strPassword).c_str(),
													ValueToString(l_strExtractDirectory).c_str(),true);
		return false;
	}
	return true;
}

void	cChoiceGirlPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("GirlScroller")
	{
		m_pScroller = cTabPage::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pScroller )
		{
			m_pScroller->SetOutOfWorking(false);
			m_pScroller->Init();
		}
	}
	else
	COMPARE_VALUE("UnlockGirlHintImage")
	{
		cRenderObject*l_pImage = GetRenderObjectBehavior(e_pTiXmlElement->FirstChildElement());
		if( l_pImage )
		{
			for( int i=1;i<m_pScroller->Count();++i )
			{
				if( !cStageClearFile::IsGirlUnlock(m_pScroller->GetObject(i)->GetName()) )
				{
					cRenderObject*l_pClone = dynamic_cast<cRenderObject*>(l_pImage->Clone());
					l_pClone->Init();
					l_pClone->SetParent(m_pScroller->GetObject(i),false);
					m_GirlUnlokImageVector.AddObjectNeglectExist(l_pClone);
				}
			}
			SAFE_DELETE(l_pImage);
		}
	}
	else
	COMPARE_VALUE("Button")
	{
		m_MainfunctionButton.AddButton(e_pTiXmlElement->FirstChildElement());
	}
	else
	COMPARE_VALUE("BGAnimation")
	{
		m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pBGMPDI )
			m_pBGMPDI->Init();
	}
	else
	COMPARE_VALUE("FileCheck")
	{
		m_pCheckFileXmlElement = new TiXmlElement(*e_pTiXmlElement);
		//<FileCheck>
		//	<Girl2 CheckFileName="GameApp/Video/Girl2Stage5Strip.fmv" DownloadURL="http://path.whgame.com.tw/Girl2.zip" Title="Girl2" Description="VideoFile" SaveName="Girl2.zip" Password="" DeleteDownloadFile="1" ExtractDirectory="GameApp\Video\"/>
		//	<Girl3 CheckFileName="GameApp/Video/Girl3Stage5Strip.fmv" DownloadURL="http://path.whgame.com.tw/Girl3.zip" Title="Girl3" Description="VideoFile" SaveName="Girl2.zip" Password="" DeleteDownloadFile="1" ExtractDirectory="GameApp\Video\"/>
		//	<Girl4 CheckFileName="GameApp/Video/Girl4Stage5Strip.fmv" DownloadURL="http://path.whgame.com.tw/Girl4.zip" Title="Girl4" Description="VideoFile" SaveName="Girl2.zip" Password="" DeleteDownloadFile="1" ExtractDirectory="GameApp\Video\"/>
		//	<Girl5 CheckFileName="GameApp/Video/Girl5Stage5Strip.fmv" DownloadURL="http://path.whgame.com.tw/Girl5.zip" Title="Girl5" Description="VideoFile" SaveName="Girl2.zip" Password="" DeleteDownloadFile="1" ExtractDirectory="GameApp\Video\"/>
		//</FileCheck>
	}
	else
	COMPARE_VALUE("cPhasePopUpMessager")
	{
		m_pDownloadPopupMessageXmlElement = new TiXmlElement(*e_pTiXmlElement);
	}
}

void	cChoiceGirlPhase::Init()
{
	m_ResourceStamp.StampResource1();
	m_bSatisfiedCondition = false;
	//
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_MainfunctionButton.Init();
	//
	m_ResourceStamp.StampResource2();
}

void	cChoiceGirlPhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if( m_pDownloadContentPhase )
	{
		m_pDownloadContentPhase->Update(e_fElpaseTime);
		if( m_pDownloadContentPhase->IsSatisfiedCondition() )
		{
			if(m_pDownloadContentPhase->IsDownloadComplete())
			{
				this->m_bSatisfiedCondition = true;
			}
			else
			{
				SAFE_DELETE(m_pDownloadContentPhase);
			}
		}
	}
	else
	{
		if( m_pScroller )
		{
			if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied())
			{
				this->m_bSatisfiedCondition = true;
				if( m_pScroller->IsChancelButtonSatisfied() )
				{
					cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
					m_strNextPhaseName = MAIN_PHASE_NAME;
				}
				else
				{
					int	l_iSelectedIndex = m_pScroller->GetCurrentWorkingObjectIndex();
					std::wstring	l_strUnlockGirlName;
					if(l_iSelectedIndex != -1)
					{
						if( l_iSelectedIndex > 0 )
							l_strUnlockGirlName = m_pScroller->GetObject(l_iSelectedIndex)->GetName();
						m_strSelectedGirlName = m_pScroller->GetCurrentWorkingObjectName();
					}
					if(  l_iSelectedIndex == 0 || cStageClearFile::IsGirlUnlock(l_strUnlockGirlName.c_str()) )
					{
						m_strNextPhaseName = CHOICE_GIRL_STAGE_PHASE_NAME;
						cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
						m_pScroller->SetSatisfiedCondition(false);
						if(!DoCheckVideoFile())
						{
							m_bSatisfiedCondition = false;
						}
					}
					else
					{//play sound and go to store
						cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
						//m_strNextPhaseName = STORE_PHASE_NAME;
						cBluffingGirlApp::WakePopupSystemMessenger(L"BuyGirl",this->GetName());					
						this->m_bSatisfiedCondition = false;
						m_pScroller->SetSatisfiedCondition(false);
					}
				}
			}
			else
				m_pScroller->Update(e_fElpaseTime);
		}
		m_MainfunctionButton.Update(e_fElpaseTime);
		if( m_MainfunctionButton.IsSatisfiedCondition() )
		{
			m_strNextPhaseName = m_MainfunctionButton.GetNextPhaseName();
			this->m_bSatisfiedCondition = true;
			m_MainfunctionButton.Init();
			cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
		}
	}
	if(this->m_bSatisfiedCondition && 
		wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) &&
		wcscmp(SETUP_PHASE_NAME,m_strNextPhaseName.c_str()))
	{		
		Destroy();
	}
}

void	cChoiceGirlPhase::Render()
{
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pScroller )
		m_pScroller->Render();
	m_MainfunctionButton.Render();
	if(m_pDownloadContentPhase)
	{
		GLRender::RenderFilledRectangle(Vector2::Zero,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,Vector4(0.1f,0.1f,0.1f,0.7f),0);
		m_pDownloadContentPhase->Render();
	}
}

void	cChoiceGirlPhase::DebugRender()
{
	if( m_pScroller )
		m_pScroller->DebugRender();
	m_MainfunctionButton.DebugRender();
}

void	cChoiceGirlPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if(m_pDownloadContentPhase)
	{
		m_pDownloadContentPhase->MouseDown(e_iPosX,e_iPosY);
	}
	else
	{
		if( m_pScroller )
			m_pScroller->MouseDown(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseDown(e_iPosX,e_iPosY);
	}
}

void	cChoiceGirlPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if(m_pDownloadContentPhase)
	{
		m_pDownloadContentPhase->MouseUp(e_iPosX,e_iPosY);
	}
	else
	{
		if( m_pScroller )
			m_pScroller->MouseUp(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseUp(e_iPosX,e_iPosY);
	}
}

void	cChoiceGirlPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if(m_pDownloadContentPhase)
	{
		m_pDownloadContentPhase->MouseMove(e_iPosX,e_iPosY);
	}
	else
	{
		if( m_pScroller )
			m_pScroller->MouseMove(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseMove(e_iPosX,e_iPosY);
	}
}

const   WCHAR*	cChoiceGirlPhase::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

void*	cChoiceGirlPhase::GetData()
{
	return (void*)m_strSelectedGirlName.c_str();
}

const WCHAR*	cChoiceGirlPhase::CurrentSelectGirlName()
{
	if( m_pScroller )
		return m_pScroller->GetCurrentWorkingObjectName();
	return 0;
}

void	cChoiceGirlPhase::RecheckUnLockGirl()
{
	if( m_GirlUnlokImageVector.Count() )
	{
		cRenderObject*l_pImage = dynamic_cast<cRenderObject*>(m_GirlUnlokImageVector[0]->Clone());
		m_GirlUnlokImageVector.Destroy();
		if( l_pImage )
		{
			for( int i=1;i<m_pScroller->Count();++i )
			{
				if( !cStageClearFile::IsGirlUnlock(m_pScroller->GetObject(i)->GetName()) )
				{
					cRenderObject*l_pClone = dynamic_cast<cRenderObject*>(l_pImage->Clone());
					l_pClone->Init();
					l_pClone->SetParent(m_pScroller->GetObject(i),false);
					m_GirlUnlokImageVector.AddObjectNeglectExist(l_pClone);
				}
			}
			SAFE_DELETE(l_pImage);
		}
	}
}