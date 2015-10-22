#include "stdafx.h"
#include "MapPointInfo.h"


	
sMapPointData::sMapPointData()
{
	vPos = Vector2::Zero;
}
sMapPointData::sMapPointData(sMapPointData*e_pMapPointData)
{
	vPos = e_pMapPointData->vPos;
}
sMapPointData::~sMapPointData()
{
	
}

sMapPointData*	sMapPointData::GetMapPointData(TiXmlElement*e_pTiXmlElement,cMapPointInfo*e_pMapPointInfo)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	sMapPointData*l_pMapPointData = nullptr;
	COMPARE_VALUE("TradeRoutesPointData")
	{
		l_pMapPointData = new sTradeRoutesPointData(e_pTiXmlElement);
		e_pMapPointInfo->m_ePointType = ePointType::ePT_POINT;
	}
	else
	COMPARE_VALUE("sHarborData")
	{
		l_pMapPointData = new sHarborData(e_pTiXmlElement);
		e_pMapPointInfo->m_ePointType = ePointType::ePT_HARBOR;
	}
	return l_pMapPointData;
}


sTradeRoutesPointData::sTradeRoutesPointData()
{
//JunctionPointNameVectorData;
}
sTradeRoutesPointData::sTradeRoutesPointData(sTradeRoutesPointData*e_pMapPointData)
{
	JunctionPointNameVectorData = e_pMapPointData->JunctionPointNameVectorData;
}

sTradeRoutesPointData::sTradeRoutesPointData(TiXmlElement*e_pTiXmlElement)
{
	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,L"TradeRoutesPointData");
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( l_pTiXmlElement )
	{
		sJunctionPointData	l_sJunctionPointData;
		PARSE_ELEMENT_START(l_pTiXmlElement)
			COMPARE_NAME("MapName")
			{
				l_sJunctionPointData.strMapName = l_strValue;
				l_sJunctionPointData.m_iMapID = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("PointName")
			{
				l_sJunctionPointData.SetPointName(l_strValue);
			}
		PARSE_NAME_VALUE_END
		this->JunctionPointNameVectorData.push_back(l_sJunctionPointData);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}

TiXmlElement*	sTradeRoutesPointData::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"TradeRoutesPointData");
	size_t	l_uiSize = JunctionPointNameVectorData.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		sJunctionPointData*l_psJunctionPointData = &JunctionPointNameVectorData[i];
		TiXmlElement*l_pJunctionPointData = new TiXmlElement(L"JunctionPointData");
		l_pJunctionPointData->SetAttribute(L"MapName",l_psJunctionPointData->strMapName.c_str());
		l_pJunctionPointData->SetAttribute(L"PointName",l_psJunctionPointData->strPointName.c_str());
		l_pTiXmlElement->LinkEndChild(l_pJunctionPointData);
	}
	return l_pTiXmlElement;
}

sTradeRoutesPointData::~sTradeRoutesPointData()
{

}

void	sTradeRoutesPointData::DebugRender(int e_iPosX,int e_iPosY)
{
	size_t	l_uiSize = this->JunctionPointNameVectorData.size();
	std::wstring	l_str;
	for(size_t i=0;i<l_uiSize;++i  )
	{
		sJunctionPointData*l_pJunctionPointData = &JunctionPointNameVectorData[i];
		l_str += l_pJunctionPointData->strMapName;
		l_str += L",";
		l_str += l_pJunctionPointData->strPointName;
		l_str += L"_";
	}
	cGameApp::RenderFont(e_iPosX,e_iPosY,l_str.c_str());
	
}

sMapPointData*sTradeRoutesPointData::Clone()
{
	sMapPointData*l_pMapPointData = new sTradeRoutesPointData(this);
	return l_pMapPointData;
}

	
sHarborData::sHarborData()
{
	m_HarborItemInfoVector.SetFromResource(true);
	m_iKingdomID = 0;
	m_AreaID = 0;
//cNamedTypedObjectVector<cHarborItemInfo>	m_HarborItemInfoVector;
}
sHarborData::sHarborData(sHarborData*e_pHarborData)
{
	m_HarborItemInfoVector.SetFromResource(true);
	m_HarborItemInfoVector = e_pHarborData->m_HarborItemInfoVector;
	m_iKingdomID = e_pHarborData->m_iKingdomID;
	m_AreaID = e_pHarborData->m_AreaID;
}

sHarborData::sHarborData(TiXmlElement*e_pTiXmlElement)
{
	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,L"sHarborData");
	m_HarborItemInfoVector.SetFromResource(true);
	m_iKingdomID = 0;
	m_AreaID = 0;
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	const WCHAR*l_strAreaID = l_pTiXmlElement->Attribute(L"AreaID");
	const WCHAR*l_strKingdomID = l_pTiXmlElement->Attribute(L"KingdomID");
	if( l_strAreaID )
		m_AreaID = GetInt(l_strAreaID);
	if( l_strKingdomID )
		m_iKingdomID = GetInt(l_strKingdomID);
	while( l_pTiXmlElement )
	{
		const WCHAR*l_strValue = l_pTiXmlElement->Value();
		COMPARE_VALUE("Commodities")
		{
			const WCHAR*l_strID = l_pTiXmlElement->Attribute(L"ID");
			const WCHAR*l_strType = l_pTiXmlElement->Attribute(L"Type");
			std::vector<int>l_CommodityID = GetValueListByCommaDivide<int>(l_strID);
			std::vector<int>l_CommodityType = GetValueListByCommaDivide<int>(l_strType);
			size_t l_uiSize = l_CommodityID.size();
			for( size_t i=0;i<l_uiSize;++i )
			{
				cCommodity*l_pCommodity = g_pCommodityManager->GetCommodity(l_CommodityID[i],(eCommodityStandType)l_CommodityType[i]);
				if( l_pCommodity )
					m_HarborItemInfoVector.AddObjectNeglectExist(l_pCommodity);
			}
			
		}
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}

TiXmlElement*	sHarborData::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"sHarborData");
	l_pTiXmlElement->SetAttribute(L"AreaID",m_AreaID);
	l_pTiXmlElement->SetAttribute(L"KingdomID",m_iKingdomID);
	int	l_iSize = m_HarborItemInfoVector.Count();
	std::wstring	l_strID;
	std::wstring	l_strType;
	TiXmlElement*l_pCommoditiesID = new TiXmlElement(L"Commodities");
	for( int i=0;i<l_iSize;++i )
	{
		cCommodity*l_pCommodity = m_HarborItemInfoVector[i];
		l_strID += ValueToStringW(l_pCommodity->GetID());
		l_strType += ValueToStringW((int)l_pCommodity->GetCommodityStandType());
		if( i != l_iSize-1 )
		{
			l_strID += L",";
			l_strType += L",";
		}
	}
	l_pCommoditiesID->SetAttribute(L"ID",l_strID.c_str());
	l_pCommoditiesID->SetAttribute(L"Type",l_strType.c_str());
	l_pTiXmlElement->LinkEndChild(l_pCommoditiesID);
	return l_pTiXmlElement;
}

bool	sHarborData::IsContainCommodity(cCommodity*e_pCommodity)
{
	int	l_iId = e_pCommodity->GetID();
	int	l_iCount = m_HarborItemInfoVector.Count();
	for(int i=0;i<l_iCount;++i)
	{
		if(l_iId == m_HarborItemInfoVector[i]->GetID())
			return true;
	}
	return false;
}

std::vector<int>	sHarborData::GetAllCommodityID()
{
	std::vector<int>l_IDVector;
	int	l_iCount = m_HarborItemInfoVector.Count();
	for( int i=0;i<l_iCount;++i )
	{
		l_IDVector.push_back(m_HarborItemInfoVector[i]->GetID());
	}
	return l_IDVector;
	
}

sHarborData::~sHarborData()
{

}

void	sHarborData::DebugRender(int e_iPosX,int e_iPosY)
{
	int	l_iSize = m_HarborItemInfoVector.Count();
	if( l_iSize > 0 )
	{
		std::string	l_str;
		for(int i=0;i<l_iSize;++i  )
		{
			cCommodity*l_pCommodity = this->m_HarborItemInfoVector[i];
			l_str += l_pCommodity->GetCharName();
			l_str += ",";
			l_str += ValueToString(l_pCommodity->GetID());
			l_str += ":";
		}
		cGameApp::RenderFont(e_iPosX,e_iPosY,ValueToStringW(l_str).c_str());
	}
}

sMapPointData*sHarborData::Clone()
{
	sMapPointData*l_pMapPointData = new sHarborData(this);
	return l_pMapPointData;
}


cMapPointInfo::cMapPointInfo()
{
	m_pMapPointData = nullptr;
}
cMapPointInfo::~cMapPointInfo()
{
	SAFE_DELETE(m_pMapPointData);
}

cMapPointInfo::cMapPointInfo(TiXmlElement*e_pTiXmlElement)
{
	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,L"HarborInfo");
	const WCHAR*l_strType = e_pTiXmlElement->Attribute(L"PointType");
	const WCHAR*l_strPos = e_pTiXmlElement->Attribute(L"Pos");
	const WCHAR*l_strName = e_pTiXmlElement->Attribute(L"Name");
	Vector2	l_vPos = Vector2::Zero;
	if( l_strPos )
	{
		l_vPos = GetVector2(l_strPos);
	}
	if( l_strType )
	{
		ePointType l_ePointType = (ePointType)GetInt(l_strType);
		this->m_pMapPointData = sMapPointData::GetMapPointData(e_pTiXmlElement->FirstChildElement(),this);
	}
	if( l_strName )
	{
		this->SetName(l_strName);
		m_iID = GetInt(l_strName);
	}
	else
	{
		this->SetName(ValueToStringW(this->GetUniqueID()).c_str());
	}
	if( m_pMapPointData )
		m_pMapPointData->vPos = l_vPos;
}

TiXmlElement*	cMapPointInfo::ToTiXmlElement()
{
	TiXmlElement*	l_pHarborInfoElement = new TiXmlElement(L"HarborInfo");
	l_pHarborInfoElement->SetAttribute(L"Name",this->GetName());
	l_pHarborInfoElement->SetAttribute(L"PointType",(int)this->m_ePointType);
	l_pHarborInfoElement->SetAttribute(L"Pos",ValueToStringW(m_pMapPointData->vPos));
	{
		TiXmlElement*l_pPointDataElement = m_pMapPointData->ToTiXmlElement();
		l_pHarborInfoElement->LinkEndChild(l_pPointDataElement);
	}
	return l_pHarborInfoElement;
}

void	cMapPointInfo::SetPointType(ePointType e_ePointType)
{
	m_ePointType = e_ePointType;
	SAFE_DELETE(m_pMapPointData);
	if( m_ePointType == ePointType::ePT_HARBOR )
		m_pMapPointData = new sHarborData();
	else
	if( m_ePointType == ePointType::ePT_POINT )
		m_pMapPointData = new sTradeRoutesPointData();
}

void	cMapPointInfo::SetPointType(ePointType e_ePointType,sHarborData*e_pHarborData)
{
	m_ePointType = e_ePointType;
	SAFE_DELETE(m_pMapPointData);
	if( e_pHarborData )
	{
		m_pMapPointData = e_pHarborData->Clone();
	}
}

bool	cMapPointInfo::IsCollided(int e_iPosX,int e_iPosY)
{
	sSphere	l_sSphere;
	l_sSphere.vCenter = Vector3(this->m_pMapPointData->vPos.x,this->m_pMapPointData->vPos.y,0);
	l_sSphere.fRadius = 30.f;
	return SphereCollidePoint(Vector3((float)e_iPosX,(float)e_iPosY,0),l_sSphere);
}

void	cMapPointInfo::DebugRender()
{
	if( m_pMapPointData )
	{
		if(this->m_ePointType == ePointType::ePT_HARBOR)
			GLRender::RenderPoint(m_pMapPointData->vPos,15,Vector4::Red);
		else
			GLRender::RenderPoint(m_pMapPointData->vPos,15,Vector4(1,1,0,1));
		this->m_pMapPointData->DebugRender((int)this->m_pMapPointData->vPos.x,(int)this->m_pMapPointData->vPos.y+30);
		cGameApp::RenderFont((int)this->m_pMapPointData->vPos.x,(int)this->m_pMapPointData->vPos.y,this->GetName());
	}
}