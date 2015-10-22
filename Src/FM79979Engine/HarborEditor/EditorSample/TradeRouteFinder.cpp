#include "stdafx.h"
#include "TradeRouteFinder.h"
#include "assert.h"


#include "windows.h"

//#include "../common/core/Utility/LogBridge.h"

namespace cscommon
{

	STradeRoute* CTradeRouteList::getTargetRouteFromTime(float fCurrentTime, float& fUseTimeForReturnRoute) const
	{
		float fRouteTime=0.f;
		float	l_fTime = 0.f;
		size_t	l_uiSize = size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			fRouteTime=(*this)[i]->getTime();

			l_fTime += fRouteTime;
			if( fCurrentTime <= l_fTime )
			{
				fUseTimeForReturnRoute=fRouteTime-(l_fTime-fCurrentTime);

				return (*this)[i];
			}
		}

		fUseTimeForReturnRoute=0.f;

		return nullptr;
	}

	 float CTradeRouteList::getTotalTime() const
	 {
		 float fTime=0.f;
		 for(int i=0; i<(int)size(); ++i) 
		 {
			 fTime+=(*this)[i]->getTime();
		 }
		 return fTime;
	 }

	 float CTradeRouteList::getTotalDistance() const
	 {
		 float fDistance=0.f;
		 for(uint32_t i=0; i<(uint32_t)size(); ++i)
			 fDistance+=(*this)[i]->getDistance();

		 return fDistance;
	 }

	 STradeRoute* CTradeRouteList::getLastRoute() const
	 {
		 if(size()==0) return nullptr;

		 return (*this)[size()-1];
	 }

	void	DumpInfo(std::string e_strInfo)
	{
#ifdef USE_WINDOW
#ifdef WIN32
#ifdef _DEBUG
		OutputDebugString(e_strInfo.c_str());
#endif
#endif
#endif
	}
	std::wstring CharToWchar(const char *e_strChar)
	{
		std::wstring	l_strResult;
		std::string		l_strForCopy = e_strChar;
		l_strResult.assign(l_strForCopy.begin(),l_strForCopy.end());
		return l_strResult;
		//if here occured crush,in the dot net that's because fucking MS
		//using critical section to lock
		//please call this after whole windos form is showed
		//or using DNCT::OpenfileGet Name.
	}

	std::string	ValueToString(int e_iValue)
	{
		char	l_temp[260];
		sprintf_s(l_temp,"%d\0",e_iValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	//please call
	std::vector<SMapData*>*g_pMapDataVector = nullptr;
	//
	struct sRegionMapAllRoutesRelaedFinder;
	std::vector<sRegionMapAllRoutesRelaedFinder*>*	g_pRegionMapAllRoutesRelaedFinderVector;


	int							GetMapIndexByID(int e_iMapID);
	SMapData*					GetMapByID(int e_iID);
	std::vector<int>			GetRelatedMapFromStartToEnd(int e_iCurrentRegionMap,int e_iTargetRegionMap);
	SPortData*					GetPortDataFromNextMapName(int e_iCurrentRegionMap,int e_iTargetRegionMap);
	template<class T> int		GetObjectIndexByPointer(std::vector<T*>*e_pVector,T*e_pObject);

	SPortData::SPortData()
	{
		iType = iID = -1;
		pParentMap = nullptr;

	}
	SPortData::~SPortData()
	{

	}

	bool	SPortData::isGoToTargetMap(int e_iMapName)
	{
		size_t	l_uiSize = ToNextMapDataVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if(ToNextMapDataVector[i].nMapID == e_iMapName)
				return true;
		}
		return false;
	}

	int		SPortData::getToTargetMapPortID(int e_iMapName)
	{
		size_t	l_uiSize = ToNextMapDataVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if(ToNextMapDataVector[i].nMapID == e_iMapName)
				return ToNextMapDataVector[i].nPortID;
		}
		return -1;
	}

	SMapData::~SMapData()
	{
		while(vecPortData.size())
		{
			delete vecPortData[0];
			vecPortData.erase(vecPortData.begin());
		}
		while(vecTradeRoute.size())
		{
			delete vecTradeRoute[0];
			vecTradeRoute.erase(vecTradeRoute.begin());
		}
	}

	STradeRoute*	SMapData::getTradeRoute(int e_iStartID,int e_iEndID)
	{
		size_t	l_iCount = vecTradeRoute.size();
		for( size_t i=0;i<l_iCount;++i )
		{
			if(vecTradeRoute[i]->iEndPortID == e_iEndID &&
				vecTradeRoute[i]->iStartPortID == e_iStartID)
			{
				return vecTradeRoute[i];
			}
		}	
		return nullptr;
	}

	SPortData*	SMapData::getPortData(int e_iID)
	{
		size_t	l_iCount = vecPortData.size();
		for( size_t i=0;i<l_iCount;++i )
		{
			if(vecPortData[i]->iID == e_iID)
			{
				return vecPortData[i];
			}
		}	
		return nullptr;	
	}

	void	SMapData::getTradeRouteByStartID(int e_iStartID,std::vector<STradeRoute*>*e_pTradeRoute)
	{
		size_t	l_iCount = vecTradeRoute.size();
		for( size_t i=0;i<l_iCount;++i )
		{
			if(vecTradeRoute[i]->iStartPortID == e_iStartID)
			{
				e_pTradeRoute->push_back(vecTradeRoute[i]);
			}
		}	
	}

	int							GetMapIndexByID(int e_iMapID)
	{
		if( g_pMapDataVector == nullptr)
			return -1;
		size_t	l_uiSize = g_pMapDataVector->size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if((*g_pMapDataVector)[i]->iMapID == e_iMapID)
			{
				return (int)i;
			}
		}
		return -1;
	}

	SMapData*		GetMapByID(int e_iID)
	{
		if( g_pMapDataVector == nullptr)
			return nullptr;
		size_t	l_uiSize = g_pMapDataVector->size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if((*g_pMapDataVector)[i]->iMapID == e_iID)
			{
				return (*g_pMapDataVector)[i];
			}
		}
		return nullptr;
	}

	sRegionMapAllRoutesRelaedFinder*GetRegionMapAllRoutesRelaedFinderByName(int e_iMapID)
	{
		int	l_iIndex = GetMapIndexByID(e_iMapID);
		//assert(l_iIndex != -1&&"the map is not exists! check data");
		if( l_iIndex == -1 )
			return nullptr;
		return (*g_pRegionMapAllRoutesRelaedFinderVector)[l_iIndex];
	}

	std::vector<int>	GetRelatedMapFromStartToEnd(int e_iCurrentRegionMap,int e_iTargetRegionMap)
	{
		std::vector<int>	l_strResult;
		if( e_iCurrentRegionMap == e_iTargetRegionMap )
		{
			return l_strResult;
		}
		if( g_pMapDataVector )
		{
			int	l_iStartIndex = GetMapIndexByID(e_iCurrentRegionMap);
			int	l_iEndIndex = GetMapIndexByID(e_iTargetRegionMap);
			if( l_iStartIndex < l_iEndIndex )
				++l_iEndIndex;
			else
				if( l_iStartIndex > l_iEndIndex )
					--l_iEndIndex;
			if( l_iStartIndex < l_iEndIndex )
			{
				for (int i = l_iStartIndex; i < l_iEndIndex; i++)
				{
					l_strResult.push_back((*g_pMapDataVector)[i]->iMapID);
				}
			}
			else
				if( l_iStartIndex > l_iEndIndex )
				{
					for (int i = l_iStartIndex; i >l_iEndIndex; --i)
					{
						l_strResult.push_back((*g_pMapDataVector)[i]->iMapID);
					}		
				}
		}
		return l_strResult;
	}

	SPortData*				GetPortDataFromNextMapName(int e_iCurrentRegionMap,int e_iTargetRegionMapp)
	{
		int l_iIndex = GetMapIndexByID(e_iCurrentRegionMap);
		if( l_iIndex != -1 )
		{
			SMapData*l_pMapData = (*g_pMapDataVector)[l_iIndex];
			size_t	l_uiSize = l_pMapData->vecPortData.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if(l_pMapData->vecPortData[i]->isGoToTargetMap(e_iTargetRegionMapp))
				{
					return l_pMapData->vecPortData[i];
				}
			}
		}
		return nullptr;
	}

	template<class T> int		GetObjectIndexByPointer(std::vector<T*>*e_pVector,T*e_pObject)
	{
		int l_iSize = (int)e_pVector->size();
		for (int i = 0; i < l_iSize; i++)
		{
			if((*e_pVector)[i] == e_pObject)
				return i;
		}
		return -1;
	}

	class  sCurrentAndPossibleRoutes
	{
		void									CopyListPointer(std::vector<sCurrentAndPossibleRoutes*>*e_Src,std::vector<sCurrentAndPossibleRoutes*>*e_Dest)
		{
			size_t l_uiSize = e_Src->size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				e_Dest->push_back((*e_Src)[i]);
			}
		}
	public:
		std::vector<sCurrentAndPossibleRoutes*>	m_ObjectVector;
		SPortData*								pThisMapPointInfo;
		//the routes
		std::vector<STradeRoute*>				RelatedAllRoutes;
		int										m_iHitRelatedIndex;
		sCurrentAndPossibleRoutes(){pThisMapPointInfo = nullptr;m_iHitRelatedIndex = -1;}
		~sCurrentAndPossibleRoutes(){};

		bool		FindTargetNode(SPortData*e_pTarget,std::vector<sCurrentAndPossibleRoutes*>*e_pParentPath,std::vector<std::vector<sCurrentAndPossibleRoutes*>* >*e_pResultPathes,bool e_bFindShoest)
		{
			e_pParentPath->push_back(this);
			//only need shorest path so compare it
			if( e_bFindShoest && e_pResultPathes->size() )
			{
				size_t	l_iCount = (*e_pResultPathes)[0]->size();
				if( e_pParentPath->size() > l_iCount )
					return false;
			}
			bool	l_TargetSame = false;
			if( e_pTarget == this->pThisMapPointInfo )
			{
				l_TargetSame = true;
				m_iHitRelatedIndex = 79979;
			}
			if( l_TargetSame == false )
			{
				size_t	l_uiSize = RelatedAllRoutes.size();
				for (size_t i = 0; i < l_uiSize; i++)
				{
					if(RelatedAllRoutes[i]->iEndPortID == e_pTarget->iID)
					{
						l_TargetSame = true;
						m_iHitRelatedIndex = i;
						break;
					}
				}
			}
			if( l_TargetSame )
			{//u could compare count if the count is bigger than the path we found,ignore it,so we could find the smallest one

				if( e_bFindShoest  )
				{
					if( e_pResultPathes->size() > 0 )
					{
						int	l_iCount = (*e_pResultPathes)[0]->size();
						int	l_iNewPathCount = (int)e_pParentPath->size();
						//only need shorest
						if( l_iNewPathCount > l_iCount )
							return false;
						if( l_iNewPathCount < l_iCount )
						{//clear old data just need new one.
							e_pResultPathes->clear();
						}
						//if same count
						//do nothing and add it again
					}
				}
				std::vector<sCurrentAndPossibleRoutes*>*l_pResult = new std::vector<sCurrentAndPossibleRoutes*>;
				CopyListPointer(e_pParentPath,l_pResult);
				e_pResultPathes->push_back(l_pResult);
				return true;
			}

			int	l_iCount = (int)this->m_ObjectVector.size();
			for( int i=0;i<l_iCount;++i )
			{
				sCurrentAndPossibleRoutes*l_pChild = this->m_ObjectVector[i];
				if( GetObjectIndexByPointer<sCurrentAndPossibleRoutes>(e_pParentPath,l_pChild) != -1 )
					continue;
				std::vector<sCurrentAndPossibleRoutes*>l_ObjectListByName;
				CopyListPointer(e_pParentPath,&l_ObjectListByName);
				if(l_pChild->FindTargetNode(e_pTarget,&l_ObjectListByName,e_pResultPathes,e_bFindShoest))
				{

				}
			}
			return false;
		}

		bool			FindTargetNode(SPortData*e_pTarget,std::vector<std::vector<sCurrentAndPossibleRoutes*>* >*e_pResultPathes,bool e_bFindShoest)
		{
			bool	l_bFind = false;
			//linked object
			int	l_iCount = (int)m_ObjectVector.size();
			for( int i=0;i<l_iCount;++i )
			{
				sCurrentAndPossibleRoutes*l_pChild = this->m_ObjectVector[i];
				std::vector<sCurrentAndPossibleRoutes*> l_ObjectListByName;
				l_ObjectListByName.push_back(this);
				if( l_pChild->FindTargetNode(e_pTarget,&l_ObjectListByName,e_pResultPathes,e_bFindShoest) )
				{
					l_bFind = true;
				}
			}
			return l_bFind;	
		}

		STradeRoute*	GetHitTradeRoutes(int e_iLastID)
		{
			if( m_iHitRelatedIndex == 79979 )
			{
				//assert(pThisMapPointInfo->pParentMap&&"sTradeRoute*	GetHitTradeRoutes(int e_iLastID) error,pParentMap not set");
				if( g_pMapDataVector && pThisMapPointInfo && pThisMapPointInfo->pParentMap )
				{
					SMapData*l_pMapData = nullptr;
					size_t	l_uiSize = g_pMapDataVector->size();
					for (size_t i = 0; i < l_uiSize; i++)
					{
						if((*g_pMapDataVector)[i]->iMapID == pThisMapPointInfo->pParentMap->iMapID)
						{
							l_pMapData = (*g_pMapDataVector)[i];
							break;
						}
					}
					if( l_pMapData )
					{
						return l_pMapData->getTradeRoute(e_iLastID,this->pThisMapPointInfo->iID);
					}
				}
			}
			if( m_iHitRelatedIndex != -1 && m_iHitRelatedIndex < (int)RelatedAllRoutes.size() )
			{
				return this->RelatedAllRoutes[m_iHitRelatedIndex];
			}
			return nullptr;
		}

		STradeRoute*	GetHitTradeRoutes(int e_iStartId,int e_iEndID)
		{
			if( this->pThisMapPointInfo->iID == e_iStartId )
			{
				size_t	l_uiSize = RelatedAllRoutes.size();
				for(size_t i=0;i<l_uiSize;++i)
				{
					if(e_iEndID == RelatedAllRoutes[i]->iEndPortID)
						return RelatedAllRoutes[i];
				}
			}
			return nullptr;
		}
	};


	struct sRegionMapAllRoutesRelaedFinder
	{
		std::vector<sCurrentAndPossibleRoutes*>		AllCurrentAndPossibleRoutes;
		SMapData*									pMapData;
		sRegionMapAllRoutesRelaedFinder(int e_iMapName)
		{
			pMapData = GetMapByID(e_iMapName);
			if( pMapData )
			{
				std::vector<SPortData*>*l_pPortDataVector = &pMapData->vecPortData;
				int	l_iCount = l_pPortDataVector->size();
				//generate all port data and related trade routes
				for (int i = 0; i < l_iCount; i++)
				{
					SPortData*l_pPortData = (*l_pPortDataVector)[i];
					sCurrentAndPossibleRoutes*l_pCurrentAndPossibleRoutes = new sCurrentAndPossibleRoutes();
					l_pCurrentAndPossibleRoutes->pThisMapPointInfo = l_pPortData;
					if(l_pPortData->iID == 1017 )
					{
						int a=0;
					}
					pMapData->getTradeRouteByStartID(l_pPortData->iID,&l_pCurrentAndPossibleRoutes->RelatedAllRoutes);
					AllCurrentAndPossibleRoutes.push_back(l_pCurrentAndPossibleRoutes);
				}
				for (int i = 0; i < l_iCount; i++)
				{
					sCurrentAndPossibleRoutes*l_pNow = AllCurrentAndPossibleRoutes[i];
					for (int j = 0; j < l_iCount; j++)
					{
						if( i != j )
						{
							size_t	l_uiRelatedRoutes = l_pNow->RelatedAllRoutes.size();
							for (size_t k = 0; k < l_uiRelatedRoutes; k++)
							{
								if(l_pNow->RelatedAllRoutes[k]->iEndPortID == AllCurrentAndPossibleRoutes[j]->pThisMapPointInfo->iID)
								{
									l_pNow->m_ObjectVector.push_back(AllCurrentAndPossibleRoutes[j]);
									if( l_pNow->m_ObjectVector.size() > 5 )
									{
										int a=0;
									}
									break;
								}
							}
						}
					}
				}
			}
		}
		~sRegionMapAllRoutesRelaedFinder()
		{
			while( AllCurrentAndPossibleRoutes.size() )
			{
				delete AllCurrentAndPossibleRoutes[0];
				AllCurrentAndPossibleRoutes.erase(AllCurrentAndPossibleRoutes.begin());
			}
		}

		void	DumpTradeInfo(STradeRoute*e_pRoute)
		{
#ifdef WIN32
#ifdef _DEBUG
			char	l_str[260];
			sprintf_s(l_str,"%d,%d",e_pRoute->iStartPortID,e_pRoute->iEndPortID);
			DumpInfo(l_str);
			DumpInfo(" __ ");	
#endif
#endif
		}

		void	GenerateRoutes(int e_iStartPoint,int e_iEndPoint,std::vector<STradeRoute*>*e_pResult)
		{
			if( pMapData )
			{
				STradeRoute*l_pTradeRoute = pMapData->getTradeRoute(e_iStartPoint,e_iEndPoint);
				if( l_pTradeRoute )
				{
					e_pResult->push_back(l_pTradeRoute);
					return;
				}
			}
			SPortData*l_pStartHarbor = pMapData->getPortData(e_iStartPoint);
			SPortData*l_pEndHarbor = pMapData->getPortData(e_iEndPoint);
			std::vector<sCurrentAndPossibleRoutes*>l_AllPossibleRelatedPath;
			size_t	l_uiSize = AllCurrentAndPossibleRoutes.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if(l_pStartHarbor == AllCurrentAndPossibleRoutes[i]->pThisMapPointInfo)
				{
					l_AllPossibleRelatedPath.push_back(AllCurrentAndPossibleRoutes[i]);
				}
			}
			l_uiSize = l_AllPossibleRelatedPath.size();
			float	l_fMinTime = 799999999.f;
			int	l_iMinIndex = -1;
			std::vector< std::vector<STradeRoute*>* >l_PossibleRoutes;
			int	l_iLastID = e_iStartPoint;
			for (size_t i = 0; i < l_uiSize; i++)
			{
				std::vector<std::vector<sCurrentAndPossibleRoutes*>* > l_ResultPathes;
				sCurrentAndPossibleRoutes*l_pStart = l_AllPossibleRelatedPath[i];
				l_pStart->FindTargetNode(l_pEndHarbor,&l_ResultPathes,true);
				float	l_fTotalTime = 0.f;
				for(size_t	j = 0;j<l_ResultPathes.size();++j)
				{
					std::vector<STradeRoute*>*l_pTradeRoutesVector = new std::vector<STradeRoute*>;
					l_PossibleRoutes.push_back(l_pTradeRoutesVector);
					DumpInfo("Path");
					DumpInfo(ValueToString(j).c_str());
					DumpInfo(":");
					std::vector<sCurrentAndPossibleRoutes*>*l_pPath = l_ResultPathes[j];
					int	l_iPathCount = (int)l_pPath->size();
					for(int	k = 1;k<l_iPathCount;++k)
					{
						int	l_iStartID = (*l_pPath)[k-1]->pThisMapPointInfo->iID;;
						int	l_iEndID = (*l_pPath)[k]->pThisMapPointInfo->iID;
						sCurrentAndPossibleRoutes*l_pCurrentAndPossibleRoutes = (*l_pPath)[k-1];
						STradeRoute*l_pTradeRoutes = l_pCurrentAndPossibleRoutes->GetHitTradeRoutes(l_iStartID,l_iEndID);
						float	l_fTime = l_pTradeRoutes->getTime();
						DumpTradeInfo(l_pTradeRoutes);
						l_fTotalTime += l_fTime;
						//find proper TradeRoute.
						l_pTradeRoutesVector->push_back(l_pTradeRoutes);
						l_iLastID = l_pCurrentAndPossibleRoutes->pThisMapPointInfo->iID;
					}
					//end route
					sCurrentAndPossibleRoutes*l_pCurrentAndPossibleRoutes = (*l_pPath)[l_iPathCount-1];
					STradeRoute*l_pTradeRoutes = l_pCurrentAndPossibleRoutes->GetHitTradeRoutes(l_iLastID);
					float	l_fTime = l_pTradeRoutes->getTime();
					l_pTradeRoutesVector->push_back(l_pTradeRoutes);
					DumpTradeInfo(l_pTradeRoutes);
					DumpInfo("\n");
					if( l_fMinTime > l_fTotalTime )
					{
						l_fMinTime = l_fTotalTime;
						l_iMinIndex = (int)i;
					}
				}
				DumpInfo("Next:\n");
			}
			if( l_iMinIndex != -1 )
			{
				std::vector<STradeRoute*>*l_pTargetRoute = l_PossibleRoutes[l_iMinIndex];
				size_t	l_uiSize2 = l_pTargetRoute->size();
				for (size_t i = 0; i < l_uiSize2; i++)
				{
					e_pResult->push_back((*l_pTargetRoute)[i]);
				}
			}
			while(l_PossibleRoutes.size())
			{
				delete l_PossibleRoutes[0];
				l_PossibleRoutes.erase(l_PossibleRoutes.begin());
			}
		}
	};

	struct sThoughMapRouteFinder
	{

		sThoughMapRouteFinder(int e_iStartMap,int e_iStartHorbarID,int e_iTargetMap,int e_iEndHarborID,std::vector<STradeRoute*>*e_pResultRoutesVector)
		{
			if( e_iStartHorbarID == e_iEndHarborID )
				return;
			std::vector<int>l_AllMap = GetRelatedMapFromStartToEnd(e_iStartMap,e_iTargetMap);
			if(l_AllMap.size() > 1  )
			{//maultimap79979
				size_t l_uiSize = l_AllMap.size()-1;
				for (size_t i = 0; i < l_uiSize; i++)
				{
					int l_strCurrentMap = l_AllMap[i];
					int l_strNextMap = l_AllMap[i+1];
					SPortData*l_pPortData = GetPortDataFromNextMapName(l_strCurrentMap,l_strNextMap);
					if(l_pPortData)
					{
						int	l_iEndHarborID = l_pPortData->iID;
						sRegionMapAllRoutesRelaedFinder*	l_pRegionMapAllRoutesRelaedFinder = GetRegionMapAllRoutesRelaedFinderByName(l_strCurrentMap);
						if( l_pRegionMapAllRoutesRelaedFinder == nullptr )
						{
							e_pResultRoutesVector->clear();
//							LB_ERROR(__FUNCTION__, " l_pRegionMapAllRoutesRelaedFinder == nullptr ", " ");
							return;
						}
						l_pRegionMapAllRoutesRelaedFinder->GenerateRoutes(e_iStartHorbarID,l_iEndHarborID,e_pResultRoutesVector);
						//this is last map find out start index

						if( l_pPortData->ToNextMapDataVector.size() ==1 )
						{
							SMapData*l_pRegionMapPointInfoManager = GetMapByID(l_strNextMap);
							if( l_pRegionMapPointInfoManager )
							{
								SPortData*l_pMapPointInfo = l_pRegionMapPointInfoManager->getPortData(l_pPortData->ToNextMapDataVector[0].nPortID);
								e_iStartHorbarID = l_pMapPointInfo->iID;
							}
						}
						else
						{
							//assert(0&&"a control point only connect to a map");
//							LB_ERROR(__FUNCTION__, " a control point only connect to a map", " ");
							e_pResultRoutesVector->clear();
							return;
						}
					}
				}
			}
			sRegionMapAllRoutesRelaedFinder	*l_pRegionMapAllRoutesRelaedFinder = GetRegionMapAllRoutesRelaedFinderByName(e_iTargetMap);
			if( l_pRegionMapAllRoutesRelaedFinder )
				l_pRegionMapAllRoutesRelaedFinder->GenerateRoutes(e_iStartHorbarID,e_iEndHarborID,e_pResultRoutesVector);
			else
			{
//				LB_ERROR(__FUNCTION__, " last RegionMapAllRoutesRelaedFinder not found", " ");
				e_pResultRoutesVector->clear();
			}
		}
	};
	//===========================================================
	//
	//===========================================================

	bool tradeRouteInit(const std::vector<SMapData*>*e_pMapDataVector)
	{
		//g_pMapDataVector = const_cast<std::vector<SMapData*>*>(e_pMapDataVector); //不得已,加了這?cast 2015/8/26 siugar
		if(g_pMapDataVector==nullptr)
			g_pMapDataVector = new std::vector<SMapData*>();
		g_pMapDataVector->assign(e_pMapDataVector->begin(), e_pMapDataVector->end());

		if( g_pMapDataVector == nullptr)
			return false;
		g_pRegionMapAllRoutesRelaedFinderVector = new std::vector<sRegionMapAllRoutesRelaedFinder*>;
		size_t	l_uiSize = g_pMapDataVector->size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			sRegionMapAllRoutesRelaedFinder* l_pRegionMapAllRoutesRelaedFinder = new sRegionMapAllRoutesRelaedFinder((*g_pMapDataVector)[i]->iMapID);
			g_pRegionMapAllRoutesRelaedFinderVector->push_back(l_pRegionMapAllRoutesRelaedFinder);
		}
		return true;
	}

	void							tradeRouteRelease()
	{
		if( g_pRegionMapAllRoutesRelaedFinderVector )
		{
			while( g_pRegionMapAllRoutesRelaedFinderVector->size() )
			{
				delete (*g_pRegionMapAllRoutesRelaedFinderVector)[0];
				g_pRegionMapAllRoutesRelaedFinderVector->erase(g_pRegionMapAllRoutesRelaedFinderVector->begin());
			}
		}

		if(g_pMapDataVector!=nullptr) {
			delete g_pMapDataVector;
			g_pMapDataVector=nullptr;
		}
	}

	void		getRoute(int iStartMapName, int e_iPortStartID, int iEndMapName, int e_iPortEndID, CTradeRouteList* e_pReceived)
	{
		sThoughMapRouteFinder(iStartMapName,e_iPortStartID,iEndMapName,e_iPortEndID,e_pReceived);
	}

	float			getTimeFromRoutes(std::vector<STradeRoute*>*e_pInput)
	{
		size_t	l_uiSize = e_pInput->size();
		float	l_fDis = 0.f;
		for (size_t i = 0; i < l_uiSize; i++)
		{
			//l_fTime += (*e_pInput)[i]->getTime();
			l_fDis += (*e_pInput)[i]->fDistance;
		}
		float	l_fTime = (l_fDis/2.f)/(1+(((6-2)/2*0.1f)));
		return l_fTime;
	}

	float			getTimeFromAutoRoutes(std::vector<STradeRoute*>* pInput)
	{
		return getTimeFromRoutes(pInput)*2.5f;
	}

}