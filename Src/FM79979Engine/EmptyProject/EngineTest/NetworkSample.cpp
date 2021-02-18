#include "stdafx.h"
#include "NetworkSample.h"



#define		MESSAGE_ID_ASSIGN(TYPE,ID)TYPE():sBaseNetworkMessage(){memset(this,0,sizeof(TYPE));iMessage = ID;iSize = (int)sizeof(TYPE);}};
#define		RESULT_MESSAGE_ID_ASSIGN(TYPE,ID)TYPE():sBaseNetworkResultMessage(){memset(this,0,sizeof(TYPE));iMessage = ID;iSize = (int)sizeof(TYPE);}};


#define		LAZY_MESSAGE_HEADER_STAR(ID)						struct sNetwork_##ID:public sBaseNetworkMessage{
#define		LAZY_MESSAGE_HEADER_END(ID)							MESSAGE_ID_ASSIGN(sNetwork_##ID,ID)

#define		LAZY_RESULT_MESSAGE_HEADER_STAR(ID)					struct sNetwork_##ID:public sBaseNetworkResultMessage{
#define		LAZY_RESULT_MESSAGE_HEADER_END(ID)					RESULT_MESSAGE_ID_ASSIGN(sNetwork_##ID,ID)

#define		CAR_GO_APP_LAZY_SIZE_LOG(TYPE,YOUR_SIZE)						{auto l_strInfo = UT::ComposeMsgByFormat("Error:%s,ExpectedSize:%d,RealSize:%d", #TYPE, sizeof(TYPE), YOUR_SIZE);FMLog::Log(l_strInfo.c_str(), false);}
#define		CAR_GO_APP_DO_RETURN_FALSE_IF_SIZE_INCORRECT(TYPE,YOUR_SIZE)	if(sizeof(TYPE)!=YOUR_SIZE){ CAR_GO_APP_LAZY_SIZE_LOG(TYPE,YOUR_SIZE); return false;}


#define		LAZY_GET_NETWORK_DATA_AND_COMPARE_SIZE(TYPE)																	\
						CAR_GO_APP_DO_RETURN_FALSE_IF_SIZE_INCORRECT(sNetwork_##TYPE, e_pNetworkReceivedPacket->iSize)		\
						sNetwork_##TYPE* l_pData = (sNetwork_##TYPE*)e_pNetworkReceivedPacket->pData;

#define		UE4_LAZY_GET_DATA(TYPE)																							 \
										if (sizeof(sNetwork_##TYPE) != e_pUNetWorkMessageDelegateData->m_iSize)							 \
										{																					 \
											UE_LOG(LogTemp, Error, TEXT(#TYPE),TEXT(" size is not correct!"));				 \
											return;																	 \
										}																					 \
										sNetwork_##TYPE*l_pData = (sNetwork_##TYPE*)e_pUNetWorkMessageDelegateData->m_pData;

//for map file name
#define		MAP_NAME_ARRAY_LENGTH			40
//tcpip 1 packet default maximum size is 65k
struct sBaseNetworkMessage
{
	int		iSize;
	int		iMessage;
	//sBaseNetworkMessage(int e_iID,int e_iSize) :iMessage(e_iID) { iSize= e_iSize;	}
	sBaseNetworkMessage() { iSize = 0; iMessage = 0; }
};

struct sBaseNetworkResultMessage :public sBaseNetworkMessage
{
	int		iResultCode;//basicly 0 for false 1 for true
	sBaseNetworkResultMessage() {}
};


#pragma pack(push,1)
struct sCarRunningData
{
	double	dbAppRunningTime;
	double	dbDrivingTime;
	int64	i64LeftWheelMovingDistance;
	int64	i64RighttWheelMovingDistance;
	sCarRunningData() { memset(this, 0, sizeof(sCarRunningData)); }
};
#pragma pack(pop)


enum eCarDrivingNetworkMessage
{
	eCDNM_S2C_TELL_SERVER_WHO_YOU_ARE_REQUEST = 0,//HW ID
	eCDNM_C2S_TELL_SERVER_WHO_YOU_ARE_RESULT,//HW ID
											 //
											 eCDNM_C2S_CAR_STATUS,//tag ID
};

#pragma pack(push,1)
LAZY_MESSAGE_HEADER_STAR(eCDNM_C2S_CAR_STATUS)
	int				iCarID;
	int64	i64RFID;//can replace by nodeID but I am lazy.
	int				i1Status;//eCarSendingStatus
	int				iBattery;
	int				iLidStatus;//eLidStatus
	int				iWIFISignalStrength;
	int				iMotorMovedDistance[2];
	int				iMotorLoading[2];
	int				iMotorSpeed[2];
	int				iMotorExceptionCode;
	int				iCPUTemperature;
	int				iMotorEncoderTemperature[2];
	sCarRunningData	CarRunningData;
LAZY_MESSAGE_HEADER_END(eCDNM_C2S_CAR_STATUS)
#pragma pack(pop)



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
	FMLog::Log("CreateAsClient",false);
	//ws://echo.websocket.org
	this->CreateAsClient(1000, "192.168.31.242", false);
	//test
	g_cTC.SetTargetTime(20.f);
}


void cNetworkSample::Update(float e_fElpaseTime)
{
	cGameNetwork::Update(e_fElpaseTime);
	std::vector<sNetworkReceivedPacket*>l_DataVector = GetReceivedDataPleaseDeleteAfterUseIt();
	size_t l_uiSize = l_DataVector.size();
	for (sNetworkReceivedPacket*l_pData : l_DataVector)
	{
		FMLog::Log("receive new message",false);
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
			sNetwork_eCDNM_C2S_CAR_STATUS l_Data;
			sNetworkSendPacket l_NetworkSendPacket;
			l_NetworkSendPacket.iSize = sizeof(l_Data);
			printf(UT::ComposeMsgByFormat("packet size:%d", l_Data.iSize).c_str());
			FMLog::Log(UT::ComposeMsgByFormat("packet size:%d", l_Data.iSize).c_str(), false);
			l_NetworkSendPacket.pData = (char*)&l_Data;
			this->SendDataToServer(&l_NetworkSendPacket);
			l_NetworkSendPacket.pData = nullptr;
		}
	}
}

void cNetworkSample::Destroy()
{
	cGameNetwork::Destroy();
}