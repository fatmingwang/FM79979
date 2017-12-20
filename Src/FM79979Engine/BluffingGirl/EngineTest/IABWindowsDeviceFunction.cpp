#include "stdafx.h"
#include "IABSetup.h"
#include "MyJni.h"
#if defined(WIN32) || defined(WASM)
bool		CallIABRCreate()
{

	return true;
}

bool		AddSKUData(const char* e_strSKUID,const char* e_strName,int e_iType,bool e_bConsumable)
{
	return true;
}

bool			PurchaseProduct(const char*e_strProductionID,ePurchaseType e_ePurchaseType,char*e_strPayLoadDataForCheck)
{
	SetWaitForRespond(1);
	ItemPurchaseSuccess(e_strProductionID);
	SetWaitForRespond(0);
	return true;
}


bool		IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType)
{
	return false;
}

bool		IsWaitingForRespond()
{
	return false;
}
#endif