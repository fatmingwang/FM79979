#ifndef _COOMODITY_H_
#define _COOMODITY_H_


enum class eCommodityStandType
{
	eCST_NORMAL = 0,
	eCST_MAX
};



std::string	ValueToString(eCommodityStandType e_eCommodityStandType);
//<cCommodity ID="0" ExtraType="" StandType="" Cost="" />
class cCommodity:public NamedTypedObject
{
	int	m_iID;
	int	m_iCost;
	//from server
	int	m_iAvailableCount;
	std::string	m_strIconFileName;
	GET_SET_DEC(eCommodityStandType,m_eCommodityStandType,GetCommodityStandType,SetCommodityStandType);
	//weired design but I don't care.
	GET_SET_DEC(std::vector<int>,m_ExtractTypeVector,GetExtractTypeVector,SetExtractTypeVector);
public:
	cCommodity(int e_iID);
	cCommodity(TiXmlElement*e_pTiXmlElement);
	~cCommodity();
	DEFINE_TYPE_INFO();

	int			GetID(){return m_iID;}
	int			GetCost(){return m_iCost;}
	int*		GetAvailableCount(){return &m_iAvailableCount;}
	std::string	GetCommodityImageName(){ return m_strIconFileName; }
	void		DebugRender(int e_iPosX,int e_iPosY);
	virtual		TiXmlElement*ToTiXmlElement();
};

class cCommodityManager:public cNamedTypedObjectVector<cCommodity>,public cNodeISAX
{
	std::map< eCommodityStandType,std::vector<cCommodity*>* >	m_CommoditiesByStandType;
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
public:
	cCommodityManager();
	~cCommodityManager();
	//sorry 
	virtual	bool	AddObject(cCommodity* e_pCommodity);
	cCommodity*		GetCommodity(int e_iID);
	cCommodity*		GetCommodity(int e_iID,eCommodityStandType e_eCommodityStandType);
	virtual	bool	Export(const char*e_strFileName,bool e_bBinary = false);
};

extern cCommodityManager*g_pCommodityManager;

#endif