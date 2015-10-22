#ifndef _MAP_CAMERA_H_
#define _MAP_CAMERA_H_

class cRegionMapPointInfoManager;
class cTradeRoutesManager;
class cRegionMap:public NamedTypedObject
{
	void	GenerateRegionsButton(TiXmlElement*e_pElement);
	int		m_iID;
public:
	cRegionMap(TiXmlElement*e_pElement);
	virtual						~cRegionMap();
	cRegionMapPointInfoManager*	m_pRegionMapPointInfoManager;
	cTradeRoutesManager*		m_pTradeRoutesManager;
	cRegionMapPointInfoManager*	GetRegionMapPointInfoManager(){ return m_pRegionMapPointInfoManager; }
	int							GetID(){ return m_iID; }
};

//end _MAP_CAMERA_H_
#endif