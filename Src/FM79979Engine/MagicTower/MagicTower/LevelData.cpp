#include "stdAfx.h"
#include "LevelData.h"


cGridBehavior::cGridBehavior(cLevelLayerGridData*e_pLevelLayerGridData,POINT e_GridIndex,eMagicTowerGridType e_eMagicTowerGridType)
{
	m_bWorking = true;
	m_pLevelLayerGridData = e_pLevelLayerGridData;
	m_eMagicTowerGridType = e_eMagicTowerGridType;
	m_GridIndex = e_GridIndex;
}


cGridBehavior*	cGridBehavior::GenerateGridBehavior(cLevelLayerGridData*e_pLevelLayerGridData,POINT e_GridIndex)
{
	eMagicTowerGridType l_eMagicTowerGridType = MTGT_ROAD;
	if( e_pLevelLayerGridData->GetTemplateDataNode() )
	{
		TiXmlElement*l_pTiXmlElement = e_pLevelLayerGridData->GetTemplateDataNode()->GetDataNode();
		if( l_pTiXmlElement )
		{
			const WCHAR*l_strName = l_pTiXmlElement->Value();
			if( l_strName )
			{
				l_eMagicTowerGridType = NameToMagicTowerGridType(l_strName);
			}
		}
	}
	cGridBehavior*l_pGridBehavior = new cGridBehavior(e_pLevelLayerGridData,e_GridIndex,l_eMagicTowerGridType);
	l_pGridBehavior->SetGridIndex(e_GridIndex);
	return l_pGridBehavior; 
}

Vector3	cGridBehavior::GetPos()
{
	return m_pLevelLayerGridData->GetPos(); 
}

void	cGridBehavior::Init()
{
	m_pLevelLayerGridData->Init();
}

void	cGridBehavior::Update(float e_fElpaseTime)
{
	if( m_bWorking )
		m_pLevelLayerGridData->Update(e_fElpaseTime);
}

void	cGridBehavior::Render()
{
	if( m_bWorking )
	{
		m_pLevelLayerGridData->Render();
		cGameApp::RenderFont(m_pLevelLayerGridData->GetPos().x,m_pLevelLayerGridData->GetPos().y,ValueToStringW((int)m_eMagicTowerGridType));
	}
}

void	cGridBehavior::DebugRender()
{
	if( m_bWorking )
	{
		m_pLevelLayerGridData->DebugRender(1);
		Vector3	l_vPos = m_pLevelLayerGridData->GetPos();
		//cGameApp::m_spGlyphFontRender->SetScale(0.2f);
		//cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(l_vPos));
		//cGameApp::RenderFont(l_vPos.x,l_vPos.y+10,ValueToStringW(this->m_GridIndex));
	}
}


cGridBehaviorListForLevelLayer::cGridBehaviorListForLevelLayer(cLevelLayerList*e_pLevelLayerList,int e_iLayerIndex)
{
	POINT	l_RowAndColumn = e_pLevelLayerList->GetRowAndColumn();
	POINT	l_GridSize = e_pLevelLayerList->GetGridSize();
	int	l_iAllCount = l_RowAndColumn.x*l_RowAndColumn.y;
	m_ppGridBehaviorGridsData = new cGridBehavior*[l_iAllCount];
	for( int i=0;i<l_iAllCount;++i )
	{
		m_ppGridBehaviorGridsData[i] = 0;
	}
	cLayer*l_pLayer = e_pLevelLayerList->GetObject(e_iLayerIndex);
	l_pLayer->SetData(this);
	this->SetOwner(e_pLevelLayerList);
	this->SetOwner(l_pLayer);
	int	l_iCount2 = l_pLayer->Count();
	int	l_iTotalCount = l_RowAndColumn.x*l_RowAndColumn.y;
	for( int i=0;i<l_iCount2;++i )
	{
		cLevelLayerGridData*l_pLevelLayerGridData = l_pLayer->GetObject(i);
		Vector3	l_vPos = l_pLevelLayerGridData->GetPos();
		POINT	l_GridIndex = {(int)l_vPos.x/l_GridSize.x,(int)l_vPos.y/l_GridSize.y};
		cGridBehavior*l_pGridBehavior = cGridBehavior::GenerateGridBehavior(l_pLevelLayerGridData,l_GridIndex);
		int	l_iIndex = l_GridIndex.x+(l_GridIndex.y*l_RowAndColumn.x);
		if( l_iIndex >= l_iTotalCount )
		{
			UT::ErrorMsg(L"object position over range",L"Warning!!");
		}
		if( m_ppGridBehaviorGridsData[l_iIndex] )
		{
			assert(0&&"it is not allow same object at same grid index");
		}
		m_ppGridBehaviorGridsData[l_iIndex] = l_pGridBehavior;
	}
}

cGridBehaviorListForLevelLayer::~cGridBehaviorListForLevelLayer()
{
	//layer->layerList
	cLevelLayerList*l_pLayerList = dynamic_cast<cLevelLayerList*>(this->GetOwner()->GetOwner());
	POINT	l_RowAndColumn = l_pLayerList->GetRowAndColumn();
	int	l_iAllCount = l_RowAndColumn.x*l_RowAndColumn.y;
	for( int i=0;i<l_iAllCount;++i )
	{
		SAFE_DELETE(m_ppGridBehaviorGridsData[i]);
	}
	SAFE_DELETE(m_ppGridBehaviorGridsData);
}

cGridBehavior*	cGridBehaviorListForLevelLayer::GetGridBehavior(POINT e_GridIndex)
{
	cLevelLayerList*l_pLayerList = dynamic_cast<cLevelLayerList*>(this->GetOwner()->GetOwner());
	POINT	l_RowAndColumn = l_pLayerList->GetRowAndColumn();
	int	l_iIndex = e_GridIndex.x+(e_GridIndex.y*l_RowAndColumn.x);
	if( l_iIndex < l_RowAndColumn.x*l_RowAndColumn.y )
		return GetGridBehavior(l_iIndex);
	return 0;
}

cGridBehavior*	cGridBehaviorListForLevelLayer::GetGridBehavior(int e_iGridIndex)
{
	return m_ppGridBehaviorGridsData[e_iGridIndex];
}

void	cGridBehaviorListForLevelLayer::RenderLevelLayerGridObject()
{
	cLevelLayerList*l_pLayerList = dynamic_cast<cLevelLayerList*>(this->GetOwner()->GetOwner());
	POINT	l_RowAndColumn = l_pLayerList->GetRowAndColumn();
	int	l_iAllCount = l_RowAndColumn.x*l_RowAndColumn.y;
	for( int i=0;i<l_iAllCount;++i )
	{
		if(m_ppGridBehaviorGridsData[i])
		{
			m_ppGridBehaviorGridsData[i]->Render();
		}
	}
}

void	cGridBehaviorListForLevelLayer::RenderLevelLayerGridObject(int e_iIndex)
{
	if( m_ppGridBehaviorGridsData[e_iIndex] )
	{
		m_ppGridBehaviorGridsData[e_iIndex]->Render();
		m_ppGridBehaviorGridsData[e_iIndex]->DebugRender();
	}
	else
	{
		int a=0;
	}
}

//set all as road to default,prevent data error
cStoreyData::cStoreyData(cLevelLayerList*e_pLevelLayerData):cLevelLayerList(e_pLevelLayerData)
{
	m_RowAncolumn = e_pLevelLayerData->GetRowAndColumn();
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cLayer*l_pLayer = this->GetObject(i);
		cGridBehaviorListForLevelLayer*l_pGridBehaviorListForLevelLayer = new cGridBehaviorListForLevelLayer(this,i);
	}
}

cStoreyData::~cStoreyData()
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cLayer*l_pLayer = this->GetObject(i);
		cGridBehaviorListForLevelLayer*l_pGridBehaviorListForLevelLayer = (cGridBehaviorListForLevelLayer*)l_pLayer->GetData();
		SAFE_DELETE(l_pGridBehaviorListForLevelLayer);
		l_pLayer->SetData(0);
	}
}

void	cStoreyData::RenderLevelLayerGridObject(POINT e_Index,int e_iLayer)
{
	cLayer*l_pLayer = this->GetObject(e_iLayer);
	if( l_pLayer )
	{
		int	l_iIndex = e_Index.x+(e_Index.y*m_RowAncolumn.x);
		RenderLevelLayerGridObject(l_iIndex,e_iLayer);
	}
}

void	cStoreyData::RenderAllLevelLayerGridObject(POINT e_Index)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cLayer*l_pLayer = this->GetObject(i);
		if( l_pLayer )
		{
			int	l_iIndex = e_Index.x+(e_Index.y*m_RowAncolumn.x);
			cGridBehaviorListForLevelLayer*l_pGridBehaviorListForLevelLayer = (cGridBehaviorListForLevelLayer*)l_pLayer->GetData();
			l_pGridBehaviorListForLevelLayer->RenderLevelLayerGridObject(l_iIndex);
		}
	}
}

void	cStoreyData::RenderLevelLayerGridObject(int e_iIndex,int e_iLayer)
{
		cLayer*l_pLayer = this->GetObject(e_iLayer);
		if( l_pLayer )
		{
			cGridBehaviorListForLevelLayer*l_pGridBehaviorListForLevelLayer = (cGridBehaviorListForLevelLayer*)l_pLayer->GetData();
			l_pGridBehaviorListForLevelLayer->RenderLevelLayerGridObject(e_iIndex);
		}
}

void	cStoreyData::RenderAllLevelLayerGridObject(int e_iIndex)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		RenderLevelLayerGridObject(e_iIndex,i);
	}
}

void	cStoreyData::RenderLevelLayerGridObject()
{
	this->RenderAllObject();
	//int	l_iTotalSize = m_iRow*m_iColumn;
	//for( int i=0;i<l_iTotalSize;++i )
	//{
	//	m_pCellData[i].Render();
	//}
}

cGridBehavior*	cStoreyData::GetGridBehavior(POINT e_GridIndex,int e_iLayer)
{
	cLayer*l_pLayer = (cLayer*)this->GetObject(e_iLayer);
	cGridBehaviorListForLevelLayer*l_pGridBehaviorListForLevelLayer = (cGridBehaviorListForLevelLayer*)l_pLayer->GetData();
	return l_pGridBehaviorListForLevelLayer->GetGridBehavior(e_GridIndex);
}