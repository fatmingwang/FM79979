#ifndef _MAIN_MAP_H_
#define _MAIN_MAP_H_

#include "RegionMap.h"
#include "TradeRouteFinder.h"
//typedef	std::function<bool(const std::vector<std::string>& vecArguments)>	DoCommandFunction;

//
class cRegionMapChange:public cNamedTypedObjectVector<cRegionMap>,public cNodeISAX
{
	cRegionMap*		GenerateRegionMap(TiXmlElement*e_pElement);
	virtual	bool	MyParse(TiXmlElement*e_pRootNode);
	cNamedTypedObjectVector<cRegionMapPointInfoManager>		m_RegionMapPointVector;
public:
	std::vector<cscommon::SMapData*>*m_pMapDataVector;
	cRegionMapChange();
	virtual	~cRegionMapChange();
};

#endif