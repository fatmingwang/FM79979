#pragma once

#include "stdint.h"
#include <string>
#include <vector>

namespace cscommon
{
	struct SMapData;
	struct STradeRoute;

	class CTradeRouteList: public std::vector<STradeRoute*>
	{
		public:
			CTradeRouteList() {}
			~CTradeRouteList() {}

			STradeRoute* getTargetRouteFromTime(float fCurrentTime, float& fUseTimeForReturnRoute) const;
			float getTotalTime() const;
			float getTotalDistance() const;
			STradeRoute* getLastRoute() const;

		private:
			
	};

	struct SPortData
	{
		uint32_t	iID;
		uint32_t	iType;//0 for harbor,1 for route visit point

		//if iType is 1
		struct SToNextMapData
		{
			uint32_t	nMapID;
			uint32_t	nPortID;
			SToNextMapData(uint32_t nMapID1, uint32_t nPortID1):nMapID(nMapID1), nPortID(nPortID1) {}
		};
		SMapData*					pParentMap;
		std::vector<SToNextMapData>	ToNextMapDataVector;
		bool	isGoToTargetMap(int e_iMapName);
		int		getToTargetMapPortID(int e_iMapName);

		SPortData();
		~SPortData();
	};
	//default is 2.f
	const float	g_fCSCOMMONTestSaileSpeed = 20.f;
	struct STradeRoute
	{
		uint32_t		iStartPortID;
		uint32_t		iEndPortID;
		//float			fTime;
		float			fDistance;
		float			fEffectRatio;
		SMapData*		pParentMap;

		float getTime() const { return (fDistance*fEffectRatio/cscommon::g_fCSCOMMONTestSaileSpeed)/(1+(((60-20)/20*0.1f))); }		
		float getDistance() const { return fDistance; }

		STradeRoute():iStartPortID(0), iEndPortID(0), fDistance(0.f), fEffectRatio(1.f), pParentMap(nullptr) {}

		~STradeRoute(){}
	};

	struct SMapData
	{
		uint32_t			Next;
		uint32_t			Previous;
		uint32_t			iMapID;
		std::vector<SPortData*>		vecPortData;
		CTradeRouteList	vecTradeRoute;

		STradeRoute*				getTradeRoute(int iStartID, int iEndID);
		void						getTradeRouteByStartID(int iStartID, std::vector<STradeRoute*>* pTradeRoute);
		SPortData*					getPortData(int iID);

		SMapData():Next(0), Previous(0), iMapID(0) {}
		~SMapData();
	};

	//
	bool							tradeRouteInit(const std::vector<SMapData*>* pMapDataVector);
	void							tradeRouteRelease();
	//
	void							getRoute(int iStartMapName, int iPortStartID, int iEndMapName, int iPortEndID, CTradeRouteList* pReceived);
	float							getTimeFromRoutes(std::vector<STradeRoute*>* pInput);
	float							getTimeFromAutoRoutes(std::vector<STradeRoute*>* pInput);
	

	//sample.
	//
	//		CSCOMMON::TradeRouteInit(l_pMapDataVector);
	//		std::vector<CSCOMMON::sTradeRoute*> l_Received;
	//		CSCOMMON::GetRoute( "NorthEurope", 1017, "NorthEurope",1008,&l_Received);
	//ensure 2 sides route set.
	//				l_pTradeRoute2->fTime = l_pTradeRoute->fTime = l_pTradeRoutes->GetDistance()/CSCOMMON::g_fCSCOMMONTestSaileSpeed;
	//				l_pTradeRoute2->iEndPortID = l_pTradeRoute->iStartPortID = l_pTradeRoutes->GetStartPoint()->GetID();
	//				l_pTradeRoute2->iStartPortID = l_pTradeRoute->iEndPortID = l_pTradeRoutes->GetEndPoint()->GetID();


	//const bool	g_bTestCSCommonCode = true;
	//bool	cRegionMapChange::MyParse(TiXmlElement*e_pRootNode)
	//{
	//	std::vector<CSCOMMON::sMapData*>*l_pMapDataVector = nullptr;
	//	if( g_bTestCSCommonCode )
	//	{
	//		l_pMapDataVector = new std::vector<CSCOMMON::sMapData*>;
	//	}
	//	XMLElement*l_pXMLElement = e_pRootNode->FirstChildElement();
	//	while( l_pXMLElement )
	//	{
	//		cRegionMap*l_pRegionMap = GenerateRegionMap(l_pXMLElement);
	//		if(!this->AddObject(l_pRegionMap))
	//		{
	//			//show error message
	//			delete l_pRegionMap;
	//		}
	//		else
	//		{
	//		}
	//		l_pXMLElement = l_pXMLElement->NextSiblingElement();
	//	}
	//	if( g_bTestCSCommonCode )
	//	{
	//		int	l_iCount = this->Count();
	//		for (int i = 0; i < l_iCount; i++)
	//		{
	//			CSCOMMON::sMapData*l_pMapData = new CSCOMMON::sMapData;
	//			cRegionMap*l_pRegionMap = this->GetObject(i);
	//			l_pMapData->strName = l_pRegionMap->GetCharName();
	//			cRegionMapPointInfoManager*l_pRegionMapPointInfoManager = l_pRegionMap->GetRegionMapPointInfoManager();
	//			if( l_pRegionMapPointInfoManager )
	//			{
	//				int	l_iCount2 = l_pRegionMapPointInfoManager->Count();
	//				for (int j = 0; j < l_iCount2; j++)
	//				{
	//					cMapPointInfo*l_pMapPointInfo = l_pRegionMapPointInfoManager->GetObject(j);
	//					sTradeRoutesPointData*l_pTradeRoutesPointData = l_pMapPointInfo->GetTradeRoutesPointData();
	//					CSCOMMON::sPortData*l_pPortData = new CSCOMMON::sPortData;
	//					l_pPortData->iID = l_pMapPointInfo->GetID();
	//					l_pPortData->iType = l_pMapPointInfo->GetPointType() == ePointType::ePT_POINT?1:0;
	//					l_pPortData->pParentMap = l_pMapData;
	//					if( l_pTradeRoutesPointData )
	//					{
	//						for (size_t i = 0; i < l_pTradeRoutesPointData->JunctionPointNameVectorData.size(); i++)
	//						{
	//							cscommon::SPortData::SToNextMapData	l_ToNextMapData = {l_pTradeRoutesPointData->JunctionPointNameVectorData[i].m_iMapID,l_pTradeRoutesPointData->JunctionPointNameVectorData[i].m_iPointID};
	//							l_pPortData->ToNextMapDataVector.push_back(l_ToNextMapData);
	//						}
	//					}
	//					l_pMapData->PortDataVector.push_back(l_pPortData);
	//
	//				}
	//			}
	//			cTradeRoutesManager*l_pTradeRoutesManager = l_pRegionMap->GetTradeRoutesManager();
	//			int	l_iCount2 = l_pTradeRoutesManager->Count();
	//			for (int j = 0; j < l_iCount2; j++)
	//			{
	//				cTradeRoutes*l_pTradeRoutes = l_pTradeRoutesManager->GetObject(j);
	//				CSCOMMON::sTradeRoute*l_pTradeRoute = new CSCOMMON::sTradeRoute;
	//				CSCOMMON::sTradeRoute*l_pTradeRoute2 = new CSCOMMON::sTradeRoute;
	//				l_pTradeRoute2->fTime = l_pTradeRoute->fTime = l_pTradeRoutes->GetDistance()/CSCOMMON::g_fCSCOMMONTestSaileSpeed;
	//				l_pTradeRoute2->iEndPortID = l_pTradeRoute->iStartPortID = l_pTradeRoutes->GetStartPoint()->GetID();
	//				l_pTradeRoute2->iStartPortID = l_pTradeRoute->iEndPortID = l_pTradeRoutes->GetEndPoint()->GetID();
	//				l_pMapData->TradeRouteVector.push_back(l_pTradeRoute);
	//				l_pMapData->TradeRouteVector.push_back(l_pTradeRoute2);
	//			}
	//			if( i > 0 )
	//			{//previous
	//				cRegionMap*l_pPreviousRegionMap = this->GetObject(i-1);
	//				l_pMapData->strPreviousMapName = l_pPreviousRegionMap->GetCharName();
	//			}
	//			else
	//			if( i < l_iCount -1 )
	//			{//next
	//				cRegionMap*l_pNextRegionMap = this->GetObject(i+1);
	//				l_pMapData->strNextMapName = l_pNextRegionMap->GetCharName();
	//			}
	//			l_pMapDataVector->push_back(l_pMapData);
	//		}
	//	}
	//	if( g_bTestCSCommonCode )
	//	{
	//		CSCOMMON::TradeRouteInit(l_pMapDataVector);
	//		std::vector<CSCOMMON::sTradeRoute*> l_Received;
	//		CSCOMMON::GetRoute( "NorthEurope", 1017, "NorthEurope",1008,&l_Received);
	//		int a=0;
	//	}
	//	return true;
	//}
	//end namespace CSCOMMON
}