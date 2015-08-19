#include "stdafx.h"
#include "IABSetup.h"
cIABSetup*g_pIABSetup = 0;
//void IABProductInfo(JNIEnv* env,jobject thiz,  jstring e_strSKU, jstring e_strPrice)
//{
//	const char *l_strSKU = env->GetStringUTFChars(e_strSKU, 0);
//	const char *l_strPrice = env->GetStringUTFChars(e_strPrice, 0);
//
//    env->ReleaseStringUTFChars(e_strSKU,l_strSKU);
//    env->ReleaseStringUTFChars(e_strPrice,l_strPrice);
//}

cIABSetup::cIABSetup(const char*e_strIABfileName)
{
	g_pIABSetup = this;
	m_fVersion = 1.f;
	if(this->Parse(e_strIABfileName))
	{
		SetupIABData();
		//because JAVA run before this
		//RegisterNativeFunction("util/iab/IabHelper","IABProductInfo","(Ljava/lang/String;Ljava/lang/String;)V",(void*)IABProductInfo);
	}
}

cIABSetup::~cIABSetup()
{

}

void	cIABSetup::SetupIABData()
{

	for(size_t i=0;i<m_ManagedSKUVector.size();++i )
	{
		AddSKUData(UT::WcharToChar(m_ManagedSKUVector[i].strSkuID.c_str()).c_str(),
					UT::WcharToChar(m_ManagedSKUVector[i].strName.c_str()).c_str(),(int)ePT_MANAGED,m_ManagedSKUVector[i].bCosumable);
	}
	for(size_t i=0;i<m_UnmanagedSKUVector.size();++i )
	{
		AddSKUData(UT::WcharToChar(m_UnmanagedSKUVector[i].strSkuID.c_str()).c_str(),
					UT::WcharToChar(m_UnmanagedSKUVector[i].strName.c_str()).c_str(),(int)ePT_MANAGED,m_ManagedSKUVector[i].bCosumable);
	}
	for(size_t i=0;i<m_SubscribeanagedSKUVector.size();++i )
	{
		AddSKUData(UT::WcharToChar(m_SubscribeanagedSKUVector[i].strSkuID.c_str()).c_str(),
					UT::WcharToChar(m_SubscribeanagedSKUVector[i].strName.c_str()).c_str(),(int)ePT_SUBSCRIBE,m_ManagedSKUVector[i].bCosumable);
	}
}

void	cIABSetup::ProcessData(TiXmlElement*e_pTiXmlElement,ePurchaseType e_ePurchaseType)
{
	sProductInfo	l_sProductInfo;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Name")
		{
			l_sProductInfo.strName = l_strValue;
		}
		else
		COMPARE_NAME("SkuName")
		{
			l_sProductInfo.strSkuID = l_strValue;
		}
		else
		COMPARE_NAME("Description")
		{
			l_sProductInfo.strDescription = l_strValue;
		}
		else
		COMPARE_NAME("StatusOpen")
		{
			l_sProductInfo.bStatusOpen = VALUE_TO_BOOLEAN;
		}
		else
		COMPARE_NAME("Consumable")
		{
			l_sProductInfo.bCosumable = VALUE_TO_BOOLEAN;
		}
	PARSE_NAME_VALUE_END
	l_sProductInfo.PurchaseType = e_ePurchaseType;
	switch(e_ePurchaseType)
	{
		case ePT_MANAGED:
			m_ManagedSKUVector.push_back(l_sProductInfo);
			break;
		case ePT_UNMANAGED:
			m_UnmanagedSKUVector.push_back(l_sProductInfo);
			break;
		case ePT_SUBSCRIBE:
			m_SubscribeanagedSKUVector.push_back(l_sProductInfo);
			break;
		default:
			break;
	}
}

void	cIABSetup::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("IAB")
	{
		const WCHAR*l_strVersion = e_pTiXmlElement->Attribute(L"Version");
		if( l_strVersion )
			m_fVersion = GetFloat(l_strVersion);
	}
	else
	COMPARE_VALUE("Managed")
	{
		ProcessData(e_pTiXmlElement,ePT_MANAGED);
	}
	else
	COMPARE_VALUE("Unmanaged")
	{
		ProcessData(e_pTiXmlElement,ePT_UNMANAGED);
	}
	else
	COMPARE_VALUE("Subscribe")
	{
		ProcessData(e_pTiXmlElement,ePT_SUBSCRIBE);
	}
}

void	cIABSetup::QueryPremiumProducts()
{
	for(size_t i=0;i<m_ManagedSKUVector.size();++i )
	{
	}
}

sProductInfo*cIABSetup::GetProductInfo(const WCHAR*e_strProductID)
{
	for(size_t i=0;i<m_ManagedSKUVector.size();++i )
	{
		if(!wcscmp(m_ManagedSKUVector[i].strName.c_str(),e_strProductID))
			return &m_ManagedSKUVector[i];
	}
	for(size_t i=0;i<m_UnmanagedSKUVector.size();++i )
	{
		if(!wcscmp(m_UnmanagedSKUVector[i].strName.c_str(),e_strProductID))
			return &m_UnmanagedSKUVector[i];
	}
	for(size_t i=0;i<m_SubscribeanagedSKUVector.size();++i )
	{
		if(!wcscmp(m_SubscribeanagedSKUVector[i].strName.c_str(),e_strProductID))
			return &m_SubscribeanagedSKUVector[i];
	}
	return 0;
}

bool	cIABSetup::QueryIABPremium(const WCHAR*e_strProductID)
{
	for(size_t i=0;i<m_ManagedSKUVector.size();++i )
	{
	}
	return false;
}


void	ItemPurchaseSuccess(const char*e_strProductName)
{
	cGameApp::OutputDebugInfoString("IABSetup.cpp");
	cGameApp::OutputDebugInfoString(e_strProductName);
	//cStorePhase*l_pStorePhase = (cStorePhase*)cBluffingGirlApp::GetPhase(STORE_PHASE_NAME);
	//if(l_pStorePhase && l_pStorePhase == cBluffingGirlApp::GetCurrentPhase())
	//{
	//	l_pStorePhase->MakeItemPurchase();
	//}
	//else
	//{
	//	l_pStorePhase->MakeItemPurchase(UT::CharToWchar(e_strProductName).c_str());
	//}
}

void	ItemPurchaseFailed()
{
//	cStorePhase*l_pStorePhase = (cStorePhase*)cBluffingGirlApp::GetPhase(STORE_PHASE_NAME);
//	l_pStorePhase->SetEnable(true);
}
//extern cBluffingGirlApp*g_pBluffingGirlApp;
void	SetWaitForRespond(int e_iWaitingRespond)
{
	//if( g_pBluffingGirlApp )
	//{
	//	cStorePhase*l_pStorePhase = (cStorePhase*)cBluffingGirlApp::GetPhase(STORE_PHASE_NAME);
	//	if( l_pStorePhase )
	//		l_pStorePhase->SetWaitForRespnd(e_iWaitingRespond?true:false);
	//}
}