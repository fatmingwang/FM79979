#include "stdafx.h"
#include "GameDefine.h"
#include "ShowFishCollectionPhase.h"
#include "FishCollectionScroller.h"
#include "FishDetailWindow.h"

cShowFishCollectionPhase::cShowFishCollectionPhase()
{
	SetName(SHOW_FISHING_COLLECTION_PHASE);
	m_eStatus = eS_COLLECTION_SCROLLER;
	m_pUnknowFishPIUnit = 0;
	m_pFishCollectionScroller = 0;
	m_pUIBG = 0;
	m_pBackButton = 0;
	m_pFishDetailWindow = 0;
}

cShowFishCollectionPhase::~cShowFishCollectionPhase()
{
	Destroy();
}

void	cShowFishCollectionPhase::Destroy()
{
	SAFE_DELETE(m_pBackButton);
	SAFE_DELETE(m_pFishCollectionScroller);
	SAFE_DELETE(m_pFishDetailWindow);
	m_pUIBG = 0;
}
//<FishScroller StartPos="47,202" Gap="129,129" Row="5" Column="7" CollisionRange="0,200,720,1280">
//	<Fish Name="FishA" >
//		<cPuzzleImageUnit PI="Image/Fish_Sphoto.pi" PIUnit="Fish83_Sphoto"/>
//	</Fish>
//	<Fish Name="FishB" >
//		<cPuzzleImageUnit PI="Image/Fish_Sphoto.pi" PIUnit="Fish84_Sphoto"/>
//	</Fish>
//<UnknowFish>
//	<cPuzzleImageUnit PI="Image/Fish_Sphoto.pi" PIUnit="Fishxx_Sphoto"/>
//</UnknowFish>
//</FishScroller>
void	cShowFishCollectionPhase::ProcessFishScrollerData(TiXmlElement*e_pTiXmlElement)
{
	SAFE_DELETE(m_pFishCollectionScroller);
	Vector2	l_vPos;
	Vector2	l_vGap;
	int	l_iRow = 0;
	int	l_iColumn = 0;
	Vector4	l_vCollisionRange = cGameApp::m_spOpenGLRender->m_vViewPortSize;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("StartPos")
		{
			l_vPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("Gap")
		{
			l_vGap = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("Row")
		{
			l_iRow = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Column")
		{
			l_iColumn = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("CollisionRange")
		{
			l_vCollisionRange = VALUE_TO_VECTOR4;
		}
	PARSE_NAME_VALUE_END
	m_pFishCollectionScroller = new cFishCollectionScroller(l_iRow,l_iColumn,l_vGap);
	m_pFishCollectionScroller->SetCollisionRange(l_vCollisionRange);
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(e_pTiXmlElement)
	{
		l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("UnknowFish")
		{
			m_pUnknowFishPIUnit = cPuzzleImageUnit::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		}
		else
		COMPARE_VALUE("Fish")
		{
			cPuzzleImageUnit*l_pPIUnit = cPuzzleImageUnit::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			m_pFishCollectionScroller->AddObject(l_pPIUnit,false);
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	if( m_pFishCollectionScroller )
		m_pFishCollectionScroller->Init();
}

void	cShowFishCollectionPhase::ProcessFishWindowData(TiXmlElement*e_pTiXmlElement)
{
	m_pFishDetailWindow = new cFishDetailWindow(e_pTiXmlElement);
}

void	cShowFishCollectionPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("FishScroller")
	{
		ProcessFishScrollerData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("FishWindow")
	{
		ProcessFishWindowData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("UIBG")
	{
		m_pUIBG = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pUIBG )
			m_pUIBG->Init();
	}
	else
	COMPARE_VALUE("UIButton")
	{
		m_pBackButton = cImageButton::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pBackButton )
			m_pBackButton->Init();
	}
}

void	cShowFishCollectionPhase::Init()
{
	Destroy();
	m_eStatus = eS_COLLECTION_SCROLLER;
	PARSE_FAILED_MESSAGE_BOX(this,"Data/Phase/ShowFishCollectionPhase.xml")
}

void	cShowFishCollectionPhase::Update(float e_fElpaseTime)
{
	if(m_eStatus == eS_COLLECTION_SCROLLER)
	{
		if( m_pFishCollectionScroller )
		{
			m_pFishCollectionScroller->Update(e_fElpaseTime);
			if( m_pFishCollectionScroller->IsSatisfiedCondition() )
			{
				m_eStatus = eS_DEATIL_WINDOW;
				m_pFishCollectionScroller->Init();
			}
		}
	}
	else
	if( m_eStatus == eS_DEATIL_WINDOW )
	{
		if( m_pFishDetailWindow )
		{
			m_pFishDetailWindow->Update(e_fElpaseTime);
		}
	}
	if( m_pUIBG )
		m_pUIBG->Update(e_fElpaseTime);
	if( m_pBackButton )
	{
		m_pBackButton->Update(e_fElpaseTime);
		if( m_pBackButton->IsSatisfiedCondition() )
		{
			if(m_eStatus == eS_COLLECTION_SCROLLER)
			{
				this->m_bSatisfiedCondition = true;
				this->m_strNextPhaseName = STAGE_SELECT_PHASE;
			}
			else
				m_eStatus = eS_COLLECTION_SCROLLER;
			m_pBackButton->Init();
		}
	}
}

void	cShowFishCollectionPhase::Render()
{
	if( m_pUIBG )
		m_pUIBG->Render();
	if(m_eStatus == eS_COLLECTION_SCROLLER)
	{
		if( m_pFishCollectionScroller )
			m_pFishCollectionScroller->Render();
	}
	else
	{
		if( m_pFishDetailWindow )
		{
			m_pFishDetailWindow->Render();
		}
	}
	if( m_pBackButton )
		m_pBackButton->Render();
}

void	cShowFishCollectionPhase::DebugRender()
{

}

void	cShowFishCollectionPhase::MouseDown(int e_iX,int e_iY)
{
	if( m_pFishCollectionScroller )
		m_pFishCollectionScroller->MouseDown(e_iX,e_iY);
	if( m_pBackButton )
		m_pBackButton->MouseDown(e_iX,e_iY);
	if( m_eStatus == eS_DEATIL_WINDOW )
	{
		if( m_pFishDetailWindow )
		{
			m_pFishDetailWindow->MouseDown(e_iX,e_iY);
		}
	}
}

void	cShowFishCollectionPhase::MouseMove(int e_iX,int e_iY)
{
	if( m_pFishCollectionScroller )
		m_pFishCollectionScroller->MouseMove(e_iX,e_iY);
	if( m_pBackButton )
		m_pBackButton->MouseMove(e_iX,e_iY);
	if( m_eStatus == eS_DEATIL_WINDOW )
	{
		if( m_pFishDetailWindow )
		{
			m_pFishDetailWindow->MouseMove(e_iX,e_iY);
		}
	}
}

void	cShowFishCollectionPhase::MouseUp(int e_iX,int e_iY)
{
	if( m_pFishCollectionScroller )
		m_pFishCollectionScroller->MouseUp(e_iX,e_iY);
	if( m_pBackButton )
		m_pBackButton->MouseUp(e_iX,e_iY);
	if( m_eStatus == eS_DEATIL_WINDOW )
	{
		if( m_pFishDetailWindow )
		{
			m_pFishDetailWindow->MouseUp(e_iX,e_iY);
		}
	}
}

void*	cShowFishCollectionPhase::GetData()
{
	return this;
}

void	cShowFishCollectionPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{

}