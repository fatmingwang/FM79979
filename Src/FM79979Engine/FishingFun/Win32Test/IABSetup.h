#ifndef	_IAB_SET_UP_H_
#define	_IAB_SET_UP_H_

enum ePurchaseType
{
	ePT_MANAGED = 0,
	ePT_UNMANAGED,
	ePT_SUBSCRIBE,
	ePT_MAX
};

struct	sProductInfo
{
	ePurchaseType	PurchaseType;
	std::wstring	strName;			//
	std::wstring	strSkuID;			//
	std::wstring	strDescription;		//
	bool			bStatusOpen;		//some items are open by specific periord
	bool			bCosumable;
	bool			bPurchased;
	sProductInfo(){bStatusOpen = true;bCosumable = true;bPurchased = false;PurchaseType = ePT_MAX;}
};

//<IAB Version="1.0">
//	<Managed	Name="" SkuName="" StatusOpen="true" Describtsion="" Cosumeable="1"/>
//	<Managed	Name="" SkuName="" StatusOpen="true" Describtsion="" Cosumeable="0"/>
//	<UnManaged	Name="" SkuName="" StatusOpen="true" Describtsion=""/>
//	<Subscribe	Name="" SkuName="" StatusOpen="true" Describtsion=""/>
//<IAB>

class	cIABSetup:public ISAXCallback
{
	void			ProcessData(TiXmlElement*e_pTiXmlElement,ePurchaseType e_ePurchaseType);
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	//
	void			SetupIABData();
public:
	cIABSetup(const char*e_strIABfileName);
	~cIABSetup();
	float	m_fVersion;
	std::vector<sProductInfo>	m_ManagedSKUVector;
	std::vector<sProductInfo>	m_UnmanagedSKUVector;
	std::vector<sProductInfo>	m_SubscribeanagedSKUVector;
	//call me to fetch from jni
	void						QueryPremiumProducts();
	//after call QueryPremiumProducts,QueryIABPremium is working
	bool						QueryIABPremium(const WCHAR*e_strProductID);
	sProductInfo*				GetProductInfo(const WCHAR*e_strProductID);
	//
};


bool		CallIABRCreate();
bool		FBPost();
bool		AddSKUData(const char* e_strSKUID,const char* e_strName,int e_iType,bool e_bConsumable);
bool		PurchaseProduct(const char*e_strProductionID,ePurchaseType e_ePurchaseType,char*e_strPayLoadDataForCheck);
bool		IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType);
bool		IsWaitingForRespond();
void		ItemPurchaseSuccess(const char*e_strProductName);

#endif