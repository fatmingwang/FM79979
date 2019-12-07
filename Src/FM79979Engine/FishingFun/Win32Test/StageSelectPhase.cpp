#include "stdafx.h"
#include "StageSelectPhase.h"
#include "GameDefine.h"
#include "ScrollableMapAndButton.h"
#include "StageInfoDialog.h"
#include "GameApp.h"
cStageSelectPhase::cStageSelectPhase()
{
	SetName(STAGE_SELECT_PHASE);
	m_pScrollableMapAndButton = 0;
	m_pStageInfoDialog = 0;
	m_pUIBGMPDI = 0;
	m_pUIButton = 0;
}

cStageSelectPhase::~cStageSelectPhase()
{
	Destroy();
}

//<StageSelectPhase>
//	<Camera ViewRect="" StartPos="" >
//	<cButtonDialog Description="" YesButton="IAmYes" NoButton="IAmNo" BG="BGG" StripButtonForBG="1">
//		<cMPDI MPDIList="" MPDI=""/>
//		<cImageButton ....>
//	</cButtonDialog>
	//<Stage Name="1" FishThread="1" />
	//<Stage Name="2" FishThread="2" />
//</StageSelectPhase>

//<StageSelectPhase>
//</StageSelectPhase>

void	cStageSelectPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ScrollableMapAndButton")
	{
		SAFE_DELETE(m_pScrollableMapAndButton);
		m_pScrollableMapAndButton = new cScrollableMapAndButton(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("UIBG")
	{
		m_pUIBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pUIBGMPDI )
			m_pUIBGMPDI->Init();
	}
	else
	COMPARE_VALUE("UIButton")
	{
		m_pUIButton = cButtonDialog::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pUIButton )
			m_pUIButton->Init();
	}
	else
	COMPARE_VALUE("PlayerUI")
	{
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		Vector3	l_vPos[3];
		cNumeralImage*l_pNumeralImage = 0;
		while( e_pTiXmlElement )
		{
			l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE("Reel")
			{
				cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
				if( l_pSubMPDI )
					l_vPos[2] = l_pSubMPDI->GetOriginalPointList()[0];
			}
			else
			COMPARE_VALUE("Money")
			{
				cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
				if( l_pSubMPDI )
					l_vPos[1] = l_pSubMPDI->GetOriginalPointList()[0];			
			}
			else
			COMPARE_VALUE("Gold")
			{
				cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
				if( l_pSubMPDI )
					l_vPos[0] = l_pSubMPDI->GetOriginalPointList()[0];			
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cNumeralImage::TypeID)
			{
				l_pNumeralImage = cNumeralImage::GetMe(e_pTiXmlElement);
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		cFishingFunApp::m_spPlayerData->SetRenderPos(l_vPos[0],l_vPos[1],l_vPos[2],l_pNumeralImage);
	}
}

void	cStageSelectPhase::Init()
{
	Destroy();
	m_strStageName = L"";
	PARSE_FAILED_MESSAGE_BOX(this,"Data/Phase/StageSelectPhase.xml");
}
//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
void	cStageSelectPhase::Update(float e_fElpaseTime)
{
	bool	l_bSatisfied = false;
	if(m_pScrollableMapAndButton)
	{
		m_pScrollableMapAndButton->Update(e_fElpaseTime);
		if(m_pScrollableMapAndButton->GetSelectStageName())
		{
			if( !m_pStageInfoDialog )
			{
				m_strStageName = m_pScrollableMapAndButton->GetSelectStageName();
				std::string	l_strFileName = "Data/StageData/";
				l_strFileName += ValueToString(m_strStageName);
				l_strFileName += ".xml";
				if( !UT::IsFileExists(l_strFileName.c_str()) )
					l_strFileName = "Data/StageData/Stage1.xml";
				cNodeISAX	l_Node;
				if(l_Node.ParseDataIntoXMLNode(l_strFileName.c_str()))
				{
					m_FishThreadsNameVector.clear();
					TiXmlElement*l_pFishThreadElement = l_Node.GetChildOfRootXmlElementByName(L"FishThread");
					if( l_pFishThreadElement )
					{
						const WCHAR*l_strFishThreadsNameVector = l_pFishThreadElement->FirstAttribute()->Value();
						m_FishThreadsNameVector = GetWStringListByCommaDivide(l_strFishThreadsNameVector);
					}
					TiXmlElement*l_pStageInfoDialogElement = l_Node.GetChildOfRootXmlElementByName(L"StageInfoDialog");
					TiXmlElement*l_pFishesShowProbabilityElement = l_Node.GetChildOfRootXmlElementByName(L"FishesShowProbability");
					m_pStageInfoDialog = new cStageInfoDialog(l_pStageInfoDialogElement,l_pFishesShowProbabilityElement);
				}
				else
				{
					l_bSatisfied = true;
				}
			}
			else
			{
				m_pStageInfoDialog->Update(e_fElpaseTime);
				cButtonDialog*l_pButtonDialog = m_pStageInfoDialog->GetButtonDialog();
				if(  l_pButtonDialog->IsSatisfiedCondition() )
				{
					if( l_pButtonDialog->GetYesImageButton()->IsSatisfiedCondition() )
					{
						l_bSatisfied = true;
					}
					else
					{
						m_pScrollableMapAndButton->Init();
						SAFE_DELETE(m_pStageInfoDialog);
					}
				}
			}
		}
	}
	if( l_bSatisfied )
	{
		this->m_bSatisfiedCondition = l_bSatisfied;
		m_strNextPhaseName = SELECT_FISH_ROD_FISH_THREAD_PHASE;
	}
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Update(e_fElpaseTime);
	if( m_pUIButton )
	{
		m_pUIButton->Update(e_fElpaseTime);
		if( m_pUIButton->IsSatisfiedCondition() )
		{
			if( m_pUIButton->GetCurrentWorkingObject()->IsSameName(L"Gallery") )
			{
				this->m_bSatisfiedCondition = true;
				m_strNextPhaseName = SHOW_FISHING_COLLECTION_PHASE;
			}
			m_pUIButton->Init();
		}
	}
}

void	cStageSelectPhase::Render()
{
	if(m_pScrollableMapAndButton)
	{
		m_pScrollableMapAndButton->Render();
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		if(m_pScrollableMapAndButton->GetSelectStageName())
		{
			if( m_pStageInfoDialog )
				m_pStageInfoDialog->Render();
		}
	}
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Render();
	if( m_pUIButton )
		m_pUIButton->Render();
}

void	cStageSelectPhase::Destroy()
{
	SAFE_DELETE(m_pScrollableMapAndButton);
	SAFE_DELETE(m_pStageInfoDialog);
	SAFE_DELETE(m_pUIButton);
	m_pUIBGMPDI = 0;
}

void	cStageSelectPhase::DebugRender()
{

}

void	cStageSelectPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if(m_pScrollableMapAndButton)
		m_pScrollableMapAndButton->MouseDown(e_iPosX,e_iPosY);
	if( m_pStageInfoDialog )
	{
		m_pStageInfoDialog->MouseDown(e_iPosX,e_iPosY);
	}
	if( m_pUIButton )
		m_pUIButton->MouseDown(e_iPosX,e_iPosY);
}

void	cStageSelectPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if(m_pScrollableMapAndButton)
		m_pScrollableMapAndButton->MouseMove(e_iPosX,e_iPosY);
	if( m_pStageInfoDialog )
	{
		m_pStageInfoDialog->MouseMove(e_iPosX,e_iPosY);
	}
	if( m_pUIButton )
		m_pUIButton->MouseMove(e_iPosX,e_iPosY);
}

void	cStageSelectPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if(m_pScrollableMapAndButton)
		m_pScrollableMapAndButton->MouseUp(e_iPosX,e_iPosY);
	if( m_pStageInfoDialog )
	{
		m_pStageInfoDialog->MouseUp(e_iPosX,e_iPosY);
	}
	if( m_pUIButton )
		m_pUIButton->MouseUp(e_iPosX,e_iPosY);
}

void*	cStageSelectPhase::GetData()
{
	//return (void*)m_strStageName.c_str();
	return this;
}

void	cStageSelectPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{

}