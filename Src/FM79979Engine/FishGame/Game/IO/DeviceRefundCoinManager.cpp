
#include "stdafx.h"
#include "DeviceRefundCoinManager.h"
#include "Comport/IOControlInterface.h"
#include "../Player/PlayerManager.h"
#include "RegisterManager.h"
//
//#include "GameApp.h"
cDeviceRefoundCoinManager::cDeviceRefoundCoinManager()
{
}


cDeviceRefoundCoinManager::~cDeviceRefoundCoinManager()
{
	Destroy();
}
void cDeviceRefoundCoinManager::GameInit(const char *filename, int e_iScoreToCoinOrTicketExchangeRate, int e_iRefundTicketOrCoinOrPrint, int e_iNumPlayer, cIOControlInterface*e_pIOControlInterface, cPlayerManager*e_pPlayerManager)
{
	Destroy();
	if (!e_pIOControlInterface)
		return;
	if (!e_pPlayerManager)
	{
		assert("where is the player manger!?the order is not correct!");
		return;
	}

	this->Destroy();
	int l_iScoreToCoinOrTicketExchangeRate = e_iScoreToCoinOrTicketExchangeRate;
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(filename);
	TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
	l_pElement = l_pElement->FirstChildElement();
	while (l_pElement)
	{		//player ID
		const WCHAR*l_strID = l_pElement->Attribute(L"ID");
		int l_iPlayerID = _wtoi(l_strID);
		if (l_iPlayerID >= e_pPlayerManager->Count())
			return;
		auto l_pPlayerBehavior = e_pPlayerManager->GetObject(l_iPlayerID);
		if (!l_pPlayerBehavior)
			break;
		cIOBitData*l_pPrintErrorIOBitData = nullptr;;
		cIOBitData*l_pResetIOBitData = nullptr;;
		cIOPackData*l_pSendingRefundIOPackData = nullptr;
		cIOPackData*l_pRecevingRefundIOPackData = nullptr;
		if (e_iRefundTicketOrCoinOrPrint == 0)
		{
			const WCHAR*l_strSendingCoinRefund = l_pElement->Attribute(L"SendingCoinRefund");
			l_pSendingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strSendingCoinRefund);
			const WCHAR*l_strRecevingCoinRefund = l_pElement->Attribute(L"RecevingCoinRefund");
			l_pRecevingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strRecevingCoinRefund);
			const WCHAR*l_strCoinReset = l_pElement->Attribute(L"CoinReset");
			l_pResetIOBitData = e_pIOControlInterface->GetObject(l_strCoinReset);
		}
		else
		if (e_iRefundTicketOrCoinOrPrint == 1)
		{
			const WCHAR*l_strSendingTicketRefund = l_pElement->Attribute(L"SendingTicketRefund");
			l_pSendingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strSendingTicketRefund);
			const WCHAR*l_strRecevingTicketRefund = l_pElement->Attribute(L"RecevingTicketRefund");
			l_pRecevingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strRecevingTicketRefund);
			const WCHAR*l_strTicketReset = l_pElement->Attribute(L"TicketReset");
			l_pResetIOBitData = e_pIOControlInterface->GetObject(l_strTicketReset);
		}
		else
		if (e_iRefundTicketOrCoinOrPrint == 2)
		{
			const WCHAR*l_strSendingTicketRefund = l_pElement->Attribute(L"SendingPrintRefund");
			if(l_strSendingTicketRefund)
				l_pSendingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strSendingTicketRefund);
			const WCHAR*l_strRecevingTicketRefund = l_pElement->Attribute(L"RecevingPrintRefund");
			if(l_strRecevingTicketRefund)
				l_pRecevingRefundIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strRecevingTicketRefund);
			const WCHAR*l_strTicketReset = l_pElement->Attribute(L"PrintReset");
			if(l_strTicketReset)
				l_pResetIOBitData = e_pIOControlInterface->GetObject(l_strTicketReset);
			const WCHAR*l_strPrintIOError = l_pElement->Attribute(L"PrintIOError");
			if(l_strPrintIOError)
				l_pPrintErrorIOBitData = e_pIOControlInterface->GetObject(l_strPrintIOError);
		}

		const WCHAR*l_strInsertCoinCount = l_pElement->Attribute(L"InsertCoinCount");
		cIOPackData*l_pInsertCoinIOIOPackData = (cIOPackData*)e_pIOControlInterface->GetObject(l_strInsertCoinCount);
		const WCHAR*l_strRefundButton = l_pElement->Attribute(L"RefundButton");
		cIOBitData*l_pRefundButtonIOBitDataIOBitData = e_pIOControlInterface->GetObject(l_strRefundButton);
		const WCHAR*l_strScoreAddButton = l_pElement->Attribute(L"ScoreAddButton");
		cIOBitData*l_pScoreAddButtonIOBitData = e_pIOControlInterface->GetObject(l_strScoreAddButton);
		const WCHAR*l_strScoreWashButton = l_pElement->Attribute(L"ScoreWashButton");
		cIOBitData*l_pScoreWashButtonIOBitData = e_pIOControlInterface->GetObject(l_strScoreWashButton);
		//
		assert(l_pResetIOBitData&&"l_pResetIOBitData not found!?");
		assert(l_pSendingRefundIOPackData&&"l_pSendingRefundIOPackData not found!?");
		assert(l_pRecevingRefundIOPackData&&"l_pRecevingRefundIOPackData not found!?");
		assert(l_pScoreWashButtonIOBitData&&"l_pScoreWashButtonIOBitData not found!?");
		assert(l_pScoreAddButtonIOBitData&&"l_pScoreAddButtonIOBitData not found!?");
		assert(l_pRefundButtonIOBitDataIOBitData&&"l_pRefundButtonIOBitDataIOBitData not found!?");
		assert(l_pInsertCoinIOIOPackData&&"l_pInsertCoinIOIOPackData not found!?");
		sScoreBehaviorStruct*l_pScoreBehaviorStruct = new sScoreBehaviorStruct(l_pPlayerBehavior->GetScorePointer(),
			l_pPlayerBehavior->GetCoinNotEnoughToRefundPointer(),
			l_pPlayerBehavior->GetRefundingCoinPointer(),
			l_pPlayerBehavior->GetPlayerID(),
			std::bind(&cPlayerBehavior::IsDeleteMessageOn, l_pPlayerBehavior));
		m_ScoreBehaviorStructVector.push_back(l_pScoreBehaviorStruct);
		cDeviceRefoundCoinBase*l_pDeviceRefoundCoinBase = new cDeviceRefoundCoinBase(
			l_iScoreToCoinOrTicketExchangeRate,
			l_pScoreBehaviorStruct,
			l_pInsertCoinIOIOPackData,
			l_pSendingRefundIOPackData,
			l_pRecevingRefundIOPackData,
			l_pResetIOBitData,//Ticket value
			l_pPrintErrorIOBitData,
			*l_pRefundButtonIOBitDataIOBitData->GetKeyData(),
			*l_pScoreAddButtonIOBitData->GetKeyData(),
			*l_pScoreWashButtonIOBitData->GetKeyData(),
			std::bind(&cPlayerBehavior::GetScore, l_pPlayerBehavior));//coin Insert value


		this->AddObjectNeglectExist(l_pDeviceRefoundCoinBase);
		l_pDeviceRefoundCoinBase->AddScoreChangeFunction(std::bind(&cPlayerBehavior::AddScoreByMoneyChangeType, l_pPlayerBehavior, std::placeholders::_1, std::placeholders::_2));

		//std::string	l_str = UT::ComposeMsgByFormat("ADD Object OK");
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		l_pElement = l_pElement->NextSiblingElement();
	}
	cRegisterManager::GetInstance();
}


void	cDeviceRefoundCoinManager::Update(float e_fElpaseTime)
{
		//std::string	l_str = UT::ComposeMsgByFormat("Have device");
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());	
		//g_pRefoundCoinDevice->Update(e_fElpaseTime);
		int	l_iCount = this->Count();		
		for( int i=0;i<l_iCount;++i )
		//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
		//for(int i=0;i<8;++i)
		{
			//std::string	l_str = UT::ComposeMsgByFormat("Refund-Index:%d  OK",i);
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			this->m_ObjectList[i]->Update(e_fElpaseTime);
		}
		cRegisterManager*l_pRegisterManager = cRegisterManager::GetInstance();
		if (l_pRegisterManager)
			l_pRegisterManager->Update(e_fElpaseTime);
}

void	cDeviceRefoundCoinManager::Render()
{
	int	l_iCount = this->Count();
	int l_iPosX = 300;
	int l_iPosY = 20;
	for (int i = 0; i < l_iCount; ++i)
	{
		this->m_ObjectList[i]->Render(l_iPosX, l_iPosY);
		l_iPosX += 300;
		if (i == 4)
		{
			l_iPosX = 300;
			l_iPosY = 60;
		}
	}
	//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
	//for(int i=0;i<8;++i)
		
}

void	cDeviceRefoundCoinManager::Destroy()
{
	DELETE_VECTOR(m_ScoreBehaviorStructVector);
}


bool	cDeviceRefoundCoinManager::RefundTest(int e_iPlayerIndex, int e_iCount)
{
	cDeviceRefoundCoinBase*l_pDeviceRefoundCoinBase = this->GetObject(e_iPlayerIndex);
	//l_pDeviceRefoundCoinBase
	return true;
}

bool	cDeviceRefoundCoinManager::IsRefundCoin()
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		auto l_pObject = (*this)[i];
		if (l_pObject->IsRefund())
			return true;
	}
	return false;
}