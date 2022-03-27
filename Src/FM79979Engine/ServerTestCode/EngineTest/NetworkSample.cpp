#include "stdafx.h"
#include "NetworkSample.h"
#include "MyWebsocketServer.h"
#include "Proto/addressbook.pb.h"

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
	int     a;
	char	strText[260];
	int     b;
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
	//WebSocketDestroy();
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
extern void cEasyWebSocketServerInit(const char* _strCetPath, const char* e_pKeyPath);
void cNetworkSample::Init()
{
	cEasyWebSocketServerInit("localhost.pem","openssh_key.pem");
	this->m_bUseExtraHeader = false;
	//WebSocketInit();
	cGameNetwork::Init();
	//ws://echo.websocket.org
	this->CreateAsServer(9991,true);
	FMLog::Log("CreateAsServer finish", false);

}


void cNetworkSample::Update(float e_fElpaseTime)
{
	//WebSocketUpdate(e_fElpaseTime);
	cGameNetwork::Update(e_fElpaseTime);
	std::vector<sNetworkReceivedPacket*>l_DataVector = GetReceivedDataPleaseDeleteAfterUseIt();
	size_t l_uiSize = l_DataVector.size();
	for (sNetworkReceivedPacket*l_pData : l_DataVector)
	{
		if (this->m_bUseExtraHeader)
		{
			sBaseNetworkMessage* l_pBaseNetworkMessage = (sBaseNetworkMessage*)l_pData->pData;
			auto l_Info = UT::ComposeMsgByFormat("new message ID:%d,Size:%d\n", l_pBaseNetworkMessage->iMessage, l_pData->iSize);
			FMLog::Log(l_Info.c_str(), false);
			unsigned int l_uiID = *(unsigned int*)l_pData->pData;
			if (l_pBaseNetworkMessage->iMessage == 2)
			{
				sNetwork_eCDNM_C2S_CAR_STATUS* l_pNetwork_eCDNM_C2S_CAR_STATUS = (sNetwork_eCDNM_C2S_CAR_STATUS*)l_pData->pData;
				printf(l_pNetwork_eCDNM_C2S_CAR_STATUS->strText);
				printf("\nqoo\n");
			}
			cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_uiID, l_pData);
		}
		else
		{
			if(1)
			{
				//https://krpc.github.io/krpc/communication-protocols/tcpip.html
				tutorial::AddressBook l_AddressBook;// = (tutorial::AddressBook::ParseFrom(()*)l_pData->pData;
				if (l_AddressBook.ParseFromArray(l_pData->pData, l_pData->iSize))
				{
					int l_iNumPeople = l_AddressBook.people_size();
					auto l_PeopleSize = l_AddressBook.people_size();
					for (int i = 0; i < l_PeopleSize; ++i)
					{
						auto l_People = l_AddressBook.people(i);
						int ID = l_People.id();
						auto l_strName = l_People.name();
						auto l_stremail = l_People.email();
						int a = 0;
					}
					int a = 0;
				}
				//l_AddressBook.ParseFromString(()
				//fstream input(l_pData->pData, ios::in | ios::binary);
				//if (!l_AddressBook.ParseFromIstream(&input))
				//{
				//}
			}
			//doesnt work.
			//google::protobuf::Any any;
			//if (any.ParseFromArray(l_pData->pData, l_pData->iSize))
			//{
			//	tutorial::AddressBook l_AddressBook;// = (tutorial::AddressBook::ParseFrom(()*)l_pData->pData;
			//	if(any.UnpackTo(&l_AddressBook))
			//	{
			//		auto l_uiSize = l_pData->iSize;
			//	}
			//}
		}
	}
	DELETE_VECTOR(l_DataVector);
}

void cNetworkSample::Destroy()
{
	cGameNetwork::Destroy();
}

bool cNetworkSample::SendDataToClient(SDLNetSocket e_SDLNetSocket, char* e_pData, int e_iDataLength)
{
	if (e_SDLNetSocket->NetworkType == sSDLNetTCPSocket::eNT_TCP)
	{
		return cGameNetwork::SendDataToClient(e_SDLNetSocket, e_pData, e_iDataLength);
	}
	else
	if (e_SDLNetSocket->NetworkType == sSDLNetTCPSocket::eNT_WEBSOCKET)
	{
		//return WebSocketSendData(e_SDLNetSocket, e_iDataLength, e_pData);
	}
	return false;
}

bool cNetworkSample::SendDataToAllClient(char* e_pData, int e_iDataLength)
{
	cGameNetwork::SendDataToAllClient(e_pData, e_iDataLength);
	//WebSocketSendDataToAll(e_iDataLength, e_pData);
	return true;
}
