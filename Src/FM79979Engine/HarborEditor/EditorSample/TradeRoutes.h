#ifndef _TRADE_ROUTES_H_
#define _TRADE_ROUTES_H_


class cMapPointInfo;

class cTradeRoutes:public NamedTypedObject
{
	GET_SET_DEC(cMapPointInfo*,m_pStartPoint,GetStartPoint,SetStartPoint);
	GET_SET_DEC(cMapPointInfo*,m_pEndPoint,GetEndPoint,SetEndPoint);
	//
	GET_SET_DEC(float,m_fSEEffect,GetSEEffect,SetSEEffect);
	GET_SET_DEC(float,m_fESEffect,GetESEffect,SetESEffect);
	GET_SET_DEC(float,m_fDistance,GetDistance,SetDistance);
	//this one is a reference.
	cLinerDataProcessor<Vector3>*	m_pRoute;
public:
	cTradeRoutes();
	cTradeRoutes(cTradeRoutes*e_pTradeRoutes);
	cTradeRoutes(TiXmlElement*e_pTiXmlElement);
	~cTradeRoutes();
	cLinerDataProcessor<Vector3>*	GetRoute(){return m_pRoute;}
	void							SetRoute(cLinerDataProcessor<Vector3>*e_pData);
	DEFINE_TYPE_INFO();
	float			GetRouteTraveTime(bool e_StoE,float e_fSpeed);	
	virtual			TiXmlElement*	ToTiXmlElement();

};

class cTradeRoutesManager:public cNamedTypedObjectVector<cTradeRoutes>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
public:
	cTradeRoutesManager();
	~cTradeRoutesManager();
	void	DebugRender();
	void	Export(char*e_strFileName);
};

#endif