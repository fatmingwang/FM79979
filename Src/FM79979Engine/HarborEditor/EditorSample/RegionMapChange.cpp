#include "stdafx.h"
#include "RegionMapChange.h"
#include "MapPointInfo.h"
#include "RegionMapInfo.h"
#include "TradeRoutes.h"

 
cRegionMapChange::cRegionMapChange()
{
	m_pMapDataVector = new std::vector<cscommon::SMapData*>;
}

cRegionMapChange::~cRegionMapChange()
{
	SAFE_DELETE(m_pMapDataVector);
	//m_pParentScene->removeChild(m_pMapViewCamera);
}

cRegionMap*		cRegionMapChange::GenerateRegionMap(TiXmlElement*e_pElement)
{
	cRegionMap*l_pRegionMap = new cRegionMap(e_pElement);
	return l_pRegionMap;
}

class cNewDisData :public cNodeISAX
{
public:
	struct sTradeNewDis
	{
		float	fDis;
		int		iStartID;
		int		iEndID;
	};
	std::vector<sTradeNewDis>		m_TradeNewDisVector;
	virtual	bool	MyParse(TiXmlElement*e_pRootNode)
	{
		e_pRootNode = e_pRootNode->FirstChildElement();
		while( e_pRootNode )
		{
			sTradeNewDis l_sTradeNewDis;
			PARSE_ELEMENT_START(e_pRootNode)
				COMPARE_NAME("Name")
				{
					std::vector<std::wstring>l_Data = GetWStringListByCommaDivide(l_strValue);
					assert(l_Data.size() == 2);
					l_sTradeNewDis.iStartID = GetInt(l_Data[0]);
					l_sTradeNewDis.iEndID = GetInt(l_Data[1]);
				}
				else
				COMPARE_NAME("NewDistance")
				{
					l_sTradeNewDis.fDis = VALUE_TO_FLOAT;
				}
				else
				COMPARE_NAME("Distance")
				{
			
				}
			PARSE_NAME_VALUE_END
			m_TradeNewDisVector.push_back(l_sTradeNewDis);
			e_pRootNode = e_pRootNode->NextSiblingElement();
		}
		return true;
	}
	sTradeNewDis*	GetNewDisByID(int e_iStartID,int e_iEndID)
	{
		for (size_t i = 0; i < m_TradeNewDisVector.size(); i++)
		{
			if(m_TradeNewDisVector[i].iEndID == e_iEndID &&
			m_TradeNewDisVector[i].iStartID == e_iStartID)
			{
				return &m_TradeNewDisVector[i];
			}
		}
		return nullptr;
	}
};
extern char*g_strSelectDirectory;
bool	cRegionMapChange::MyParse(TiXmlElement*e_pRootNode)
{
	cNewDisData l_cNewDisData;
	const WCHAR*l_strExtraRouteFileName = e_pRootNode->Attribute(L"ExtraRouteFileName");
	if( l_strExtraRouteFileName )
	{
		std::string	l_strfileName = g_strSelectDirectory;
		l_strfileName += "/";
		l_strfileName += UT::WcharToChar(l_strExtraRouteFileName);
		l_cNewDisData.ParseWithMyParse(l_strfileName.c_str());
	}
	TiXmlElement*l_pXMLElement = e_pRootNode->FirstChildElement();
	while( l_pXMLElement )
	{
		cRegionMap*l_pRegionMap = GenerateRegionMap(l_pXMLElement);
		if(!this->AddObject(l_pRegionMap))
		{
			//show error message
			delete l_pRegionMap;
		}
		else
		{
		}
		l_pXMLElement = l_pXMLElement->NextSiblingElement();
	}
	int	l_iCount = this->Count();
	for (int i = 0; i < l_iCount; i++)
	{
		cscommon::SMapData*l_pMapData = new cscommon::SMapData;
		cRegionMap*l_pRegionMap = this->GetObject(i);
		l_pMapData->iMapID = l_pRegionMap->GetID();
		cRegionMapPointInfoManager*l_pRegionMapPointInfoManager = l_pRegionMap->GetRegionMapPointInfoManager();
		if( l_pRegionMapPointInfoManager )
		{
			int	l_iCount2 = l_pRegionMapPointInfoManager->Count();
			for (int j = 0; j < l_iCount2; j++)
			{
				cMapPointInfo*l_pMapPointInfo = l_pRegionMapPointInfoManager->GetObject(j);
				sTradeRoutesPointData*l_pTradeRoutesPointData = l_pMapPointInfo->GetTradeRoutesPointData();
				cscommon::SPortData*l_pPortData = new cscommon::SPortData;
				l_pPortData->iID = l_pMapPointInfo->GetID();
				l_pPortData->iType = l_pMapPointInfo->GetPointType() == ePointType::ePT_POINT?1:0;
				l_pPortData->pParentMap = l_pMapData;
				if( l_pTradeRoutesPointData )
				{
					size_t	l_iii = l_pTradeRoutesPointData->JunctionPointNameVectorData.size();
					for (size_t k = 0; k <l_iii ; k++)
					{
						cscommon::SPortData::SToNextMapData	l_ToNextMapData(l_pTradeRoutesPointData->JunctionPointNameVectorData[k].m_iMapID,l_pTradeRoutesPointData->JunctionPointNameVectorData[k].m_iPointID);
						l_pPortData->ToNextMapDataVector.push_back(l_ToNextMapData);
					}
				}
				l_pMapData->vecPortData.push_back(l_pPortData);

			}
		}
		cTradeRoutesManager*l_pTradeRoutesManager = l_pRegionMap->m_pTradeRoutesManager;
		int	l_iCount2 = l_pTradeRoutesManager->Count();
		for (int j = 0; j < l_iCount2; j++)
		{
			cTradeRoutes*l_pTradeRoutes = l_pTradeRoutesManager->GetObject(j);
			cscommon::STradeRoute*l_pTradeRoute = new cscommon::STradeRoute;
			cscommon::STradeRoute*l_pTradeRoute2 = new cscommon::STradeRoute;
			//l_pTradeRoute2->fTime = l_pTradeRoute->fTime = l_pTradeRoutes->GetDistance()/cscommon::g_fCSCOMMONTestSaileSpeed;
			l_pTradeRoute2->iEndPortID = l_pTradeRoute->iStartPortID = l_pTradeRoutes->GetStartPoint()->GetID();
			l_pTradeRoute2->iStartPortID = l_pTradeRoute->iEndPortID = l_pTradeRoutes->GetEndPoint()->GetID();
			cNewDisData::sTradeNewDis*l_pNewDisData = l_cNewDisData.GetNewDisByID(l_pTradeRoute->iStartPortID,l_pTradeRoute->iEndPortID);
			if( l_pNewDisData )
				l_pTradeRoute2->fDistance = l_pTradeRoute->fDistance = l_pNewDisData->fDis;
			else
				l_pTradeRoute2->fDistance = l_pTradeRoute->fDistance = l_pTradeRoutes->GetDistance();
			l_pMapData->vecTradeRoute.push_back(l_pTradeRoute);
			l_pMapData->vecTradeRoute.push_back(l_pTradeRoute2);
		}
		if( i > 0 )
		{//previous
			cRegionMap*l_pPreviousRegionMap = this->GetObject(i-1);
			l_pMapData->Previous = l_pPreviousRegionMap->GetID();
		}
		else
		if( i < l_iCount -1 )
		{//next
			cRegionMap*l_pNextRegionMap = this->GetObject(i+1);
			l_pMapData->Next = l_pNextRegionMap->GetID();
		}
		m_pMapDataVector->push_back(l_pMapData);
	}
	cscommon::tradeRouteInit(m_pMapDataVector);
	//cscommon::CTradeRouteList	l_CTradeRouteList;
	//cscommon::getRoute( 1, 1010, 3,3005,&l_CTradeRouteList);
	return true;
}