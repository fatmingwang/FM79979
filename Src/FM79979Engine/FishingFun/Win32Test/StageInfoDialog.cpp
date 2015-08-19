#include "stdafx.h"
#include "StageInfoDialog.h"
#include "PlayerData.h"
#include "GameApp.h"
//<StageInfoDialog>
//	<cButtonDialog Description="" YesButton="IAmYes" NoButton="IAmNo" BG="BGG" StripButtonForBG="1">
//		<cMPDI MPDIList="" MPDI=""/>
//	</cButtonDialog>		
//	<Place Name="¹Ú¤Û´ò" ShowPos="">
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</Place>
//	<WaterDepth ShowPos=""/>
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</WaterDepth>
//	<WaterZone Name="³J¤ô´ò³Õ" ShowPos=""/>
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</WaterZone>
//	<FishCost ShowPos="" Cost="6"/>
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</FishCost>
//	<FishKind ShowPos="" FishKind="">
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</FishKind>
//	<FishHooked ShowPos="">
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</FishHooked>
//	<ActivedCost ShowPos="">
//		<cSubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
//	</ActivedCost>
//</StageInfoDialog>
Vector2	GetShowPos(TiXmlElement*e_pElement)
{
	Vector2	l_vPos(0,0);
	e_pElement = e_pElement->FirstChildElement();
	if(e_pElement)
	{
		cSubMPDI*l_psubMPDI = cSubMPDI::GetMe(e_pElement,false);
		if( l_psubMPDI )
		{
			Vector3	l_vPos2 = l_psubMPDI->GetOriginalPointList()[0];
			l_vPos.x = l_vPos2.x;
			l_vPos.y = l_vPos2.y;
		}
		else
		{
			UT::ErrorMsg(L"GetShowPos(TiXmlElement*e_pElement)",L"SubMPDI name error!");;
		}
	}
	return l_vPos;
}
cStageInfoDialog::cStageInfoDialog(TiXmlElement*e_pElement,TiXmlElement*e_pFishesShowProbabilityElement)
{
	m_pButtonDialog = 0;
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("cButtonDialog")
		{
			m_pButtonDialog = cButtonDialog::GetMe(e_pElement);
			if( m_pButtonDialog )
				m_pButtonDialog->Init();
		}
		else
		COMPARE_VALUE("Place")
		{
			m_strPlaceName = ATTRIBUT_VALUE(e_pElement,"Name");
			m_vPlaceNameShowPos = GetShowPos(e_pElement);
		}
		else
		COMPARE_VALUE("WaterDepth")
		{
			m_fWaterDepth = GetFloat(e_pElement->Attribute(L"Depth"));
			m_vWaterDepthShowPos = GetShowPos(e_pElement);
		}
		else
		COMPARE_VALUE("WaterZone")
		{
			m_strWaterZoneName = ATTRIBUT_VALUE(e_pElement,"Name");
			 m_vWaterZoneNameShowPos = GetShowPos(e_pElement);
			//WaterZoneName;
		}
//		else
//		COMPARE_VALUE("FishCost")
//		{
//			m_iFishCost = GetInt(e_pElement->Attribute(L"Cost"));
//			m_vFishCostShowPos = GetShowPos(e_pElement);
//		}
		else
		COMPARE_VALUE("FishKind")
		{
			m_iFishKind = GetInt(e_pElement->Attribute(L"FishKind"));
			m_vFishKindShowPos = GetShowPos(e_pElement);
		}
		else
		COMPARE_VALUE("FishHooked")
		{
			m_iFishesHooked = 0;
			//cFishingFunApp::m_spPlayerData->m_FishCollectionVector;
			//e_pFishesShowProbabilityElement
			//here should parse stage xml data and compare player's collection data to figure how many fishes has been hooked!
			m_vFishesHookedShowPos = GetShowPos(e_pElement);
		}
		else
		COMPARE_VALUE("ActivedCost")
		{
			m_iStageActiveCost = GetInt(e_pElement->Attribute(L"Cost"));
			m_vStageActiveCostShowPos = GetShowPos(e_pElement);
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

cStageInfoDialog::~cStageInfoDialog()
{
	SAFE_DELETE(m_pButtonDialog);
}


void	cStageInfoDialog::Update(float e_fElpaseTime)
{
	if( m_pButtonDialog )
		m_pButtonDialog->Update(e_fElpaseTime);
}

void	cStageInfoDialog::Render()
{
	if( m_pButtonDialog )
		m_pButtonDialog->Render();
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,0,1,1));
	cGameApp::RenderFont(m_vPlaceNameShowPos.x,m_vPlaceNameShowPos.y,m_strPlaceName);
	cGameApp::RenderFont(m_vWaterDepthShowPos.x,m_vWaterDepthShowPos.y,ValueToStringW(m_fWaterDepth));
	cGameApp::RenderFont(m_vWaterZoneNameShowPos.x,m_vWaterZoneNameShowPos.y,m_strWaterZoneName);
	//cGameApp::RenderFont(m_vFishCostShowPos.x,m_vFishCostShowPos.y,ValueToStringW(m_iFishCost));

	cGameApp::RenderFont(m_vFishKindShowPos.x,m_vFishKindShowPos.y,ValueToStringW(m_iFishKind));
	cGameApp::RenderFont(m_vFishesHookedShowPos.x,m_vFishesHookedShowPos.y,ValueToStringW(m_iFishesHooked));
	cGameApp::RenderFont(m_vStageActiveCostShowPos.x,m_vStageActiveCostShowPos.y,ValueToStringW(m_iStageActiveCost));
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));
}


void	cStageInfoDialog::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseDown(e_iPosX,e_iPosY);
}

void	cStageInfoDialog::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseMove(e_iPosX,e_iPosY);
}

void	cStageInfoDialog::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseUp(e_iPosX,e_iPosY);
}