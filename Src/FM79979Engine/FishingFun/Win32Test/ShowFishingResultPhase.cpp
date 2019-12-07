#include "stdafx.h"
#include "ShowFishingResultPhase.h"
#include "GameApp.h"
#include "GameDefine.h"
#include "GameObjectSelector.h"
#include "FishingTag.h"
#include "FishBehavior.h"
cShowFishingResultPhase::cShowFishingResultPhase()
{
	this->SetName(SHOW_FISHING_RESULT_PHASE);
	m_pScroller = 0;
	//m_pScroller = new cScroller();
	m_pTemplateElement = 0;
	m_pUIButton = 0;
}

cShowFishingResultPhase::~cShowFishingResultPhase()
{
	Destroy();
	m_CaughtFishDataVector.Destroy();
}

void	cShowFishingResultPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("UIBG")
	{
		m_pUIBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pUIBGMPDI )
			m_pUIBGMPDI->Init();
	}
	else
	COMPARE_VALUE("UIButton")
	{
		m_pUIButton = GetClickMouseBehaviorList(e_pTiXmlElement->FirstChildElement());
		if( m_pUIButton )
			m_pUIButton->Init();
	}
	else
	COMPARE_VALUE_WITH_DEFINE(cGameItemInfoButton::TypeID)
	{
		SAFE_DELETE(m_pTemplateElement);
		m_pTemplateElement = new TiXmlElement(*(TiXmlElement*)e_pTiXmlElement->Parent());
	}
}

void	cShowFishingResultPhase::Init()
{
	Destroy();
	//
	//m_pScroller = new cScroller((cPuzzleImage*)0,Vector4(160,320,530,cGameApp::m_spOpenGLRender->m_vGameResolution.y),0,0,false,0);
	m_pScroller = new cScroller((cPuzzleImage*)0,Vector4(200,300,cGameApp::m_spOpenGLRender->m_vGameResolution.x,1080),0,0,false,0);
	//
	PARSE_FAILED_MESSAGE_BOX(this,"Data/Phase/ShowFishingResult.xml");
	cFishTag*l_pTemplate = new cFishTag(m_pTemplateElement,0,Vector3::Zero);
	cFishTag*l_pDebugObject = 0;
	while( m_CaughtFishDataVector.Count() )
	{
		cCaughtFishData*l_pCaughtFishData = m_CaughtFishDataVector[0];
		cFishTag*l_pClone = new cFishTag(l_pTemplate);
		l_pClone->SetName(l_pCaughtFishData->GetName());
		l_pDebugObject = l_pClone;
		l_pClone->SetModel(l_pCaughtFishData->m_pFishModel,Vector3(235,330,0));
		l_pClone->ChangeText(0,l_pCaughtFishData->GetName());
		l_pClone->ChangeText(1,ValueToStringW(l_pCaughtFishData->m_iNumCaught));
		l_pClone->ChangeText(2,ValueToStringW((int)l_pCaughtFishData->m_fLength));
		l_pClone->ChangeText(3,ValueToStringW(l_pCaughtFishData->m_iTotalCost));
		m_pScroller->AddObject((cClickMouseBehavior*)l_pClone,true);
		m_CaughtFishDataVector.RemoveObject(0);
	}
	m_pScroller->Init();
	SAFE_DELETE(l_pTemplate);
}
//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
void	cShowFishingResultPhase::Update(float e_fElpaseTime)
{
	if(m_pScroller)
	{
		m_pScroller->Update(e_fElpaseTime);
	}
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Update(e_fElpaseTime);
	if( m_pUIButton )
	{
		m_pUIButton->Update(e_fElpaseTime);
		if( m_pUIButton->IsSatisfiedCondition() )
		{
			if( m_pUIButton->GetCurrentWorkingObject()->IsSameName(L"Back") )
			{
				this->m_bSatisfiedCondition  =true;
				this->m_strNextPhaseName = STAGE_SELECT_PHASE;
				//clear all fish model
				Destroy();
				cGameApp::m_spColladaParserVector->Destroy();

			}
			if( m_pUIButton )
				m_pUIButton->Init();
		}
	}
}

void	cShowFishingResultPhase::Render()
{
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Render();
	if( m_pUIButton )
		m_pUIButton->Render();
	if(m_pScroller)
		m_pScroller->Render();
}

void	cShowFishingResultPhase::Destroy()
{
	m_pUIBGMPDI = 0;
	SAFE_DELETE(m_pScroller);
	SAFE_DELETE(m_pTemplateElement);
	SAFE_DELETE(m_pUIButton);
}

void	cShowFishingResultPhase::DebugRender()
{

}


void*	cShowFishingResultPhase::GetData()
{
	return 0;
}

void	cShowFishingResultPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_CaughtFishDataVector.Destroy();
	cNamedTypedObjectVector<cFishBehavior>*l_pBeenHookedFishes = (cNamedTypedObjectVector<cFishBehavior>*)e_pData;
	int	l_iCount = l_pBeenHookedFishes->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cFishBehavior*l_pFishBehavior = l_pBeenHookedFishes->GetObject(i);
		cCaughtFishData*l_pCaughtFishData = m_CaughtFishDataVector.GetObject(l_pFishBehavior->GetName());
		float	l_fLength = l_pFishBehavior->GetRandomWidth();
		int	l_iCost = l_pFishBehavior->GetGiveMoneyByWidth(l_fLength);
		if(!l_pCaughtFishData)
		{
			l_pCaughtFishData = new cCaughtFishData();
			l_pCaughtFishData->SetName(l_pFishBehavior->GetName());
			l_pCaughtFishData->m_pFishModel = (cAnimationMesh*)l_pFishBehavior->GetAnimationData();
			l_pCaughtFishData->m_fLength = l_fLength;
			l_pCaughtFishData->m_iTotalCost = l_iCost;
			l_pCaughtFishData->m_iNumCaught = 1;
			m_CaughtFishDataVector.AddObjectNeglectExist(l_pCaughtFishData);
		}
		else
		{
			if( l_pCaughtFishData->m_fLength < l_fLength )
				l_pCaughtFishData->m_fLength = l_fLength;
			l_pCaughtFishData->m_iTotalCost += l_iCost;
			l_pCaughtFishData->m_iNumCaught += 1;		
		}
	}
}

void    cShowFishingResultPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pUIButton )
		m_pUIButton->MouseDown(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void    cShowFishingResultPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pUIButton )
		m_pUIButton->MouseMove(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

void    cShowFishingResultPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pUIButton )
		m_pUIButton->MouseUp(e_iPosX,e_iPosY);
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}