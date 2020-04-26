#include "stdafx.h"
#include "NetworkSample.h"

TYPDE_DEFINE_MARCO(cNetworkSample)
cNetworkSample::cNetworkSample()
{
	REG_NET_MESSAGE_FUNCTION(eGNMI_S2C_LOGIN_RESULT, &cNetworkSample::CurrentBonsNetworkEventProcess);
	SetConnectionLostCallbackFunction(std::bind(&cNetworkSample::LostConnection, this));
}

cNetworkSample::~cNetworkSample()
{
	SetConnectionLostCallbackFunction(nullptr);
}

void cNetworkSample::LostConnection()
{
}

bool cNetworkSample::CurrentBonsNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket * e_pData)
{
	return false;
}

bool cNetworkSample::LoginNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData)
{
	if (sizeof(sGNMI_C2S_LOGIN) != e_pData->iSize)
		return false;
	sGNMI_C2S_LOGIN*l_pGNMI_C2S_LOGIN = (sGNMI_C2S_LOGIN*)e_pData->pData;
	return true;
}
//test
UT::sTimeCounter g_cTC;
void cNetworkSample::Init()
{
	cGameNetwork::Init();
	this->CreateAsClient(7999, "10.168.1.120", true);
	//test
	g_cTC.SetTargetTime(3.f);
}

void cNetworkSample::Update(float e_fElpaseTime)
{
	cGameNetwork::Update(e_fElpaseTime);
	std::vector<sNetworkReceivedPacket*>l_DataVector = GetReceivedDataPleaseDeleteAfterUseIt();
	size_t l_uiSize = l_DataVector.size();
	for (sNetworkReceivedPacket*l_pData : l_DataVector)
	{
		unsigned int l_uiID = *(unsigned int*)l_pData->pData;
		cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_uiID, l_pData);
	}
	DELETE_VECTOR(l_DataVector);
	//test code
	if (this->m_pSocket)
	{
		g_cTC.Update(e_fElpaseTime);
		if (g_cTC.bTragetTimrReached)
		{
			g_cTC.Start();
			sNetworkSendPacket l_NetworkSendPacket;
			struct sTest
			{
				int a = 5;
			};
			sTest l_sTest;
			l_NetworkSendPacket.iSize = sizeof(sTest);
			l_NetworkSendPacket.pData = (char*)&l_sTest;
			this->SendDataToServer(&l_NetworkSendPacket);
			l_NetworkSendPacket.pData = nullptr;
		}
	}
}

void cNetworkSample::Destroy()
{
	cGameNetwork::Destroy();
}