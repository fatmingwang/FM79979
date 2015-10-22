#ifndef _HARBOR_INFO_H_
#define _HARBOR_INFO_H_

#include "Commodity.h"

enum ePointType
{
	ePT_HARBOR = 0,
	ePT_POINT,
	ePT_NONE,
};
class cMapPointInfo;
//any point on map could be a harbor or just a transition point of routes
struct sMapPointData
{
	Vector2 vPos;
	sMapPointData();
	sMapPointData(sMapPointData*e_pMapPointData);
	virtual~sMapPointData();
	virtual	sMapPointData*	Clone() = 0;
	virtual	void			DebugRender(int e_iPosX,int e_iPosY) = 0;
	virtual	TiXmlElement*	ToTiXmlElement() = 0;
	static	sMapPointData*	GetMapPointData(TiXmlElement*e_pTiXmlElement,cMapPointInfo*e_pMapPointInfo);
};

struct sTradeRoutesPointData:public sMapPointData
{
	struct sJunctionPointData
	{
		std::wstring	strPointName;
		std::wstring	strMapName;
		int				m_iPointID;
		int				m_iMapID;
		void			SetPointName(const WCHAR*e_strPointName)
		{
			strPointName = e_strPointName;
			m_iPointID = GetInt(e_strPointName);
		}	
	};
	std::vector<sJunctionPointData>	JunctionPointNameVectorData;
	sTradeRoutesPointData();
	sTradeRoutesPointData(TiXmlElement*e_pTiXmlElement);
	sTradeRoutesPointData(sTradeRoutesPointData*e_pMapPointData);
	virtual~sTradeRoutesPointData();
	virtual	void			DebugRender(int e_iPosX,int e_iPosY);
	virtual	sMapPointData*	Clone();
	virtual	TiXmlElement*	ToTiXmlElement();
};


struct sHarborData:public sMapPointData
{
	//the data come from CommodityManage.
	cNamedTypedObjectVector<cCommodity>	m_HarborItemInfoVector;
	int									m_iKingdomID;
	int									m_AreaID;
	sHarborData();
	sHarborData(sHarborData*e_pHarborData);
	sHarborData(TiXmlElement*e_pTiXmlElement);
	virtual~sHarborData();
	virtual	void			DebugRender(int e_iPosX,int e_iPosY);
	virtual	sMapPointData*	Clone();
	virtual	TiXmlElement*	ToTiXmlElement();
	bool					IsContainCommodity(cCommodity*e_pCommodity);
	std::vector<int>		GetAllCommodityID();
};



class cMapPointInfo:public NamedTypedObject
{
	friend struct sMapPointData;
	//GET_SET_DEC(Vector2,m_vPos
	ePointType		m_ePointType;
	sMapPointData*	m_pMapPointData;
	int				m_iID;
public:
	cMapPointInfo();
	cMapPointInfo(TiXmlElement*e_pTiXmlElement);
	virtual~cMapPointInfo();
	int				GetID(){ return m_iID; }
	virtual	TiXmlElement*	ToTiXmlElement();
	void			SetPointType(ePointType e_ePointType);
	void			SetPointType(ePointType e_ePointType,sHarborData*e_pHarborData);
	ePointType		GetPointType(){return m_ePointType;}
	sMapPointData*	GetMapPointData(){return m_pMapPointData;}
	void			DebugRender();
	bool			IsCollided(int e_iPosX,int e_iPosY);

	sTradeRoutesPointData*GetTradeRoutesPointData()
	{
		if( this->m_ePointType == ePointType::ePT_POINT )
		{
			return (sTradeRoutesPointData*)this->m_pMapPointData;
		}
		return nullptr;
	}
};


#endif