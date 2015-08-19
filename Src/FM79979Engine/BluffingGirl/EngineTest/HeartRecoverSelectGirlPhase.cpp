#include "stdafx.h"
#include "HeartRecoverSelectGirlPhase.h"
#include "StageClearFile.h"
#include "PlayerData.h"
#include "BluffingGirlApp.h"
cHeartRecoverSelectGirlPhase::cHeartRecoverSelectGirlPhase(const char*e_strFileName)
{
	this->SetName(HEART_RECOVER_SELECTGIRL_PHASE_NAME);
	m_iHeartLimit = 5;
	this->m_strFileName = e_strFileName;
	Vector4	l_vCollideRange(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	m_strNextPhaseName = HEART_RECOVER_PHASE_NAME;
	m_pScroller = 0;
	m_pBGMPDI = 0;
	m_pPraticeOrShakeSelection = 0;
	m_eUpdateType = eUT_SELECT_GIRL;
	m_bShakeOrPratice = true;
	//m_pBackButton = new cImageButton(l_pCommonMPDIList->GetObject(L"Back"));
}

cHeartRecoverSelectGirlPhase::~cHeartRecoverSelectGirlPhase()
{
	m_GirlUnlokImageVector.Destroy();
	SAFE_DELETE(m_pPraticeOrShakeSelection);
	SAFE_DELETE(m_pScroller);
}

void	cHeartRecoverSelectGirlPhase::Destroy()
{
	m_pBGMPDI = 0;
	m_MainfunctionButton.Destroy();
	SAFE_DELETE(m_pScroller);
	SAFE_DELETE(m_pPraticeOrShakeSelection);
	m_GirlUnlokImageVector.Destroy();
	m_ResourceStamp.ReleaseNewResource();
}

void	cHeartRecoverSelectGirlPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ChoiceGirlPhase")
	{
		const WCHAR*l_strHeartLimit = e_pTiXmlElement->Attribute(L"HeartLimit");
		if( l_strHeartLimit )
		{
			m_iHeartLimit = GetInt(l_strHeartLimit);
		}
	}
	else
	COMPARE_VALUE("GirlScroller")
	{
		m_pScroller = (cTabPage*)GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement());
		if( m_pScroller )
		{
			m_pScroller->SetOutOfWorking(false);
			m_pScroller->Init();
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
	COMPARE_VALUE("cTabPage")
	{
		m_pPraticeOrShakeSelection = cTabPage::GetMe(e_pTiXmlElement);
		if( m_pPraticeOrShakeSelection )
		{
			m_pPraticeOrShakeSelection->Init();
			m_pPraticeOrShakeSelection->SetOutOfWorking(false);
		}
	}
}

void	cHeartRecoverSelectGirlPhase::Init()
{
	m_ResourceStamp.StampResource1();
	m_bSatisfiedCondition = false;
	//
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_MainfunctionButton.Init();
	m_eUpdateType = eUT_SELECT_GIRL;
	//
	//cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(L"Image/Girl/ChoiceGirl.pi");
//	m_pScroller = new c3DScroller(l_pPI,Vector4(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y));
	if(cBluffingGirlApp::m_spPlayerData->HeartCount() >= m_iHeartLimit)
	{
		cImageButton*l_pImageButton = (cImageButton*)m_pPraticeOrShakeSelection->GetObject(L"Shake");
		if( l_pImageButton )
		{
			l_pImageButton->SetColor(Vector4(0.5,0.5,0.5,0.3f));
			l_pImageButton->SetEnable(false);
		}
	}
	m_ResourceStamp.StampResource2();
}

void	cHeartRecoverSelectGirlPhase::SelectGirlUpdate(float e_fElpaseTime)
{
	m_MainfunctionButton.Update(e_fElpaseTime);
	if( m_pScroller )
	{
		if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied())
		{
			if(m_pScroller->GetCurrentWorkingObjectName())
				m_strSelectedGirlName = m_pScroller->GetCurrentWorkingObjectName();
			if( m_pScroller->IsChancelButtonSatisfied() )
			{
				this->m_bSatisfiedCondition = true;
				m_strNextPhaseName = CHOICE_GIRL_PHASE_NAME;
			}
			else
			{
				int	l_iIndex = m_pScroller->GetCurrentWorkingObjectIndex();
				if(  l_iIndex == 0 || cStageClearFile::IsGirlUnlock(m_strSelectedGirlName.c_str()) )
				{
					m_eUpdateType = eUT_SELECT_SHAKE_OR_PRATICE;
					if( m_pPraticeOrShakeSelection )
					{
						if(cBluffingGirlApp::m_spPlayerData->HeartCount() >= m_iHeartLimit)
						{
							cImageButton*l_pImageButton = (cImageButton*)m_pPraticeOrShakeSelection->GetObject(L"Shake");
							if( l_pImageButton )
							{
								l_pImageButton->SetColor(Vector4(0.5,0.5,0.5,0.3f));
								l_pImageButton->SetEnable(false);
							}
						}
					}
				}
				if( m_eUpdateType != eUT_SELECT_SHAKE_OR_PRATICE )
				{//play sound and go to store

					cBluffingGirlApp::WakePopupSystemMessenger(L"BuyGirl",this->GetName());
				}
			}
			m_pScroller->SetSatisfiedCondition(false);
		}
		else
			m_pScroller->Update(e_fElpaseTime);
	}
	if( m_MainfunctionButton.IsSatisfiedCondition() )
	{
		m_strNextPhaseName = m_MainfunctionButton.GetNextPhaseName();
		m_MainfunctionButton.Init();
		this->m_bSatisfiedCondition = true;
	}
}

void	cHeartRecoverSelectGirlPhase::ShakeOrPraticeUpdate(float e_fElpaseTime)
{
	if( m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->Update(e_fElpaseTime);
		if( m_pPraticeOrShakeSelection->IsSatisfiedCondition() )
		{
			const WCHAR*l_strName = m_pPraticeOrShakeSelection->GetCurrentWorkingObjectName();
			this->m_bSatisfiedCondition = true;
			m_strNextPhaseName = HEART_RECOVER_PHASE_NAME;
			if( !wcscmp(l_strName,L"Shake") )
			{
				if(cBluffingGirlApp::m_spPlayerData->ShockTimeMinus())
				{
					m_bShakeOrPratice = true;
					m_bSatisfiedCondition = true;
				}
				else
				{//play wrong sound 
					this->m_bSatisfiedCondition = false;
					m_pPraticeOrShakeSelection->Init();
				}
			}
			else
			{
				m_bShakeOrPratice = false;
				m_bSatisfiedCondition = true;
			}
		}
		if( m_pPraticeOrShakeSelection->IsChancelButtonSatisfied() )
		{
			m_eUpdateType = eUT_SELECT_GIRL;
			m_pPraticeOrShakeSelection->Init();
		}
	}
	else
		m_bSatisfiedCondition = true;
	if( m_bSatisfiedCondition )
	{
		m_strNextPhaseName = HEART_RECOVER_PHASE_NAME;
	}
}

void	cHeartRecoverSelectGirlPhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if(m_eUpdateType == eUT_SELECT_GIRL)
	{
		SelectGirlUpdate(e_fElpaseTime);
	}
	else
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE )
	{
		ShakeOrPraticeUpdate(e_fElpaseTime);
	}
	if(this->m_bSatisfiedCondition && 
		wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) &&
		wcscmp(SETUP_PHASE_NAME,m_strNextPhaseName.c_str()))
	{
		Destroy();
	}
}

void	cHeartRecoverSelectGirlPhase::Render()
{
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pScroller )
		m_pScroller->Render();
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE && m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->Render();
	}
	m_MainfunctionButton.Render();
}

void	cHeartRecoverSelectGirlPhase::DebugRender()
{
	if( m_pScroller )
		m_pScroller->DebugRender();
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE && m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->DebugRender();
	}
	m_MainfunctionButton.DebugRender();
}

void	cHeartRecoverSelectGirlPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE && m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->MouseDown(e_iPosX,e_iPosY);
	}
	else	
	{
		if( m_pScroller )
			m_pScroller->MouseDown(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseDown(e_iPosX,e_iPosY);
	}
}

void	cHeartRecoverSelectGirlPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE && m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->MouseMove(e_iPosX,e_iPosY);
	}
	else
	{
		if( m_pScroller )
			m_pScroller->MouseMove(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseMove(e_iPosX,e_iPosY);
	}
}

void	cHeartRecoverSelectGirlPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_eUpdateType == eUT_SELECT_SHAKE_OR_PRATICE && m_pPraticeOrShakeSelection )
	{
		m_pPraticeOrShakeSelection->MouseUp(e_iPosX,e_iPosY);
	}
	else
	{
		if( m_pScroller )
			m_pScroller->MouseUp(e_iPosX,e_iPosY);
		m_MainfunctionButton.MouseUp(e_iPosX,e_iPosY);
	}
}

const   WCHAR*	cHeartRecoverSelectGirlPhase::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

void*	cHeartRecoverSelectGirlPhase::GetData()
{
	return (void*)m_strSelectedGirlName.c_str();
}

const WCHAR*	cHeartRecoverSelectGirlPhase::CurrentSelectGirlName()
{
	if( m_pScroller )
		return m_pScroller->GetCurrentWorkingObjectName();;
	return 0;
}

void	cHeartRecoverSelectGirlPhase::RecheckUnLockGirl()
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