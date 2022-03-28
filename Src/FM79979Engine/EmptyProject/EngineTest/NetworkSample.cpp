#include "stdafx.h"
#include "NetworkSample.h"

//https://blog.csdn.net/qq_42956179/article/details/118576680


TYPDE_DEFINE_MARCO(cNetworkSample)
cNetworkSample::cNetworkSample()
{
	//REG_NET_MESSAGE_FUNCTION(eGNMI_S2C_LOGIN_RESULT, &cNetworkSample::CurrentBonsNetworkEventProcess);
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
void cNetworkSample::Init()
{
	this->m_bUseExtraHeader = false;
	cGameNetwork::Init();
	FMLog::Log("CreateAsClient start123",false);
	//ws://echo.websocket.org
#ifdef WASM
	//ws://sitapi.diresoft.net/api/agent
	//wss://funplayapi.pypc.net/api/agent
	this->CreateAsClient(9992, "ws://sitapi.diresoft.net/api/agent", true,15);
	SetOpenSocketOkayCallback([](SDLNetSocket e_SDLNetSocket) 
		{
			//play connect sound.
			FMLOG("on open callback");
		});
	SetConnectionLostCallbackFunction([]()
		{
			//play lost connection sound.
			FMLOG("on lost callback");
		});
#else
	//this->CreateAsClient(9991, "192.168.31.242",true,15);
#endif
	FMLog::Log("CreateAsClient finish", false);
}


void cNetworkSample::Update(float e_fElpaseTime)
{
	int16 a = 0;
	cGameNetwork::Update(e_fElpaseTime);
	std::vector<sNetworkReceivedPacket*>l_DataVector = GetReceivedDataPleaseDeleteAfterUseIt();
	size_t l_uiSize = l_DataVector.size();
	for (sNetworkReceivedPacket*l_pData : l_DataVector)
	{
		//sBaseNetworkMessage*l_pBaseNetworkMessage = (sBaseNetworkMessage*)l_pData->pData;
		//auto l_Info = UT::ComposeMsgByFormat("new message ID:%d,Size:%d\n", l_pBaseNetworkMessage->iMessage, l_pData->iSize);
		//FMLog::Log(l_Info.c_str(),false);
		//unsigned int l_uiID = *(unsigned int*)l_pData->pData;
		//if (l_pBaseNetworkMessage->iMessage == 2)
		//{
		//	sNetwork_eCDNM_C2S_CAR_STATUS*l_pNetwork_eCDNM_C2S_CAR_STATUS = (sNetwork_eCDNM_C2S_CAR_STATUS*)l_pData->pData;
		//	printf(l_pNetwork_eCDNM_C2S_CAR_STATUS->strText);
		//	printf("\nqoo\n");
		//}
		//cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_uiID, l_pData);
	}
	DELETE_VECTOR(l_DataVector);
}

void cNetworkSample::Destroy()
{
	cGameNetwork::Destroy();
}