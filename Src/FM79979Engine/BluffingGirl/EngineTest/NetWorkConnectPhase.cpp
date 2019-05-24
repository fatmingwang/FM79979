#include "stdafx.h"
#include "BluffingGirlApp.h"
#include "GameNetwork.h"
#include "NetWorkConnectPhase.h"
#include "AllPhaseDefine.h"
#include "BluffingGameProbability.h"
bool	g_bKeyBoardChancel = false;
std::string*	g_pstrTargetIP = 0;
cNetWorkConnectPhase*g_pNetWorkConnectPhase = 0;

#define	DICE_COUNT	5

void	NewPlayerAdded(void*e_pData)
{
	int l_iNumConnected = g_pNetWorkConnectPhase->GetConnectedPlayerCount()+1;
	_TCPsocket*l_pTCPsocket = (_TCPsocket*)e_pData;
	int	l_iIndex = cBluffingGirlApp::m_spGameNetwork->GetEmptyPlayerIndex();
	cBluffingGirlApp::m_spGameNetwork->SetPlayerIDIndex(l_iIndex,l_pTCPsocket);
	sGamePacket	l_GamePacket;
	l_GamePacket.SetMessageID(eGMI_TO_CLIENT_START_ID);
	l_GamePacket.SetPlayerIndex(l_iIndex);;
	l_GamePacket.SetTotalPlayer(g_pNetWorkConnectPhase->m_iTotalPlayer);
	cBluffingGirlApp::m_spGameNetwork->SendDataToClient(l_pTCPsocket,&l_GamePacket);

	l_GamePacket.SetMessageID(eGMI_TO_CLIENT_PLAYER_COUNT);
	l_GamePacket.SetNumPlayerConnected(l_iNumConnected);
	//max support 4 player
	char	l_DiceTable[4*DICE_COUNT];
	int	l_iDiecCount = g_pNetWorkConnectPhase->m_iTotalPlayer*DICE_COUNT;
	for( int i=0;i<l_iDiecCount;++i )
	{
		l_DiceTable[i] = cBluffingGirlApp::m_spGameNetwork->m_DiceTable[i];
	}
	l_GamePacket.SetDiceTable(l_DiceTable,l_iDiecCount);
	sSendPacket l_Packet;
	l_Packet.pData = (char*)&l_GamePacket;
	l_Packet.iSize = sizeof(int)+sizeof(int);
	l_Packet.iSize += sizeof(char)*DICE_COUNT*g_pNetWorkConnectPhase->m_iTotalPlayer;
	cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_Packet);
	l_Packet.pData = 0;
	g_pNetWorkConnectPhase->SetConnectedPlayerCount(l_iNumConnected);
}

void	PlayerLeave(void*e_pData)
{
	g_pNetWorkConnectPhase->SetConnectedPlayerCount(g_pNetWorkConnectPhase->GetConnectedPlayerCount()-1);
	_TCPsocket*l_pTCPsocket = (_TCPsocket*)e_pData;
	int	l_iLeaveIndex = cBluffingGirlApp::m_spGameNetwork->GetPlayerIndex(l_pTCPsocket);
	cBluffingGirlApp::m_spGameNetwork->SetPlayerIDIndex(l_iLeaveIndex,0);
	g_pNetWorkConnectPhase->SetDoPlayerDisconnect();
	//while( g_pNetWorkConnectPhase->IsDoPlayerDisconnect() )
	//{
	//
	//}
}


std::string		ConvertIP(const char*e_strIP)
{
	for( size_t i=0;i<strlen(e_strIP);++i )
	{
		if( e_strIP[i] != '.'  )
		{
			if( (e_strIP[i] < '0' || e_strIP[i] > '9') )
			{
				std::string l_str = e_strIP;
				return l_str;
			}
		}
	}
	char	l_strOriginalIP[16];
	sprintf(l_strOriginalIP,"%s.",e_strIP);
	const char*l_strStripIP[4] = 
	{
		strtok(l_strOriginalIP,"."),
		strtok(0,"."),
		strtok(0,"."),
		strtok(0,".")
	};
	int	l_iAddLength[4] = {3-(int)strlen(l_strStripIP[0]),3- (int)strlen(l_strStripIP[1]),3- (int)strlen(l_strStripIP[2]),3- (int)strlen(l_strStripIP[3])};
	std::string	l_strIP;
	for( int i=0;i<4;++i )
	{
		for( int j=0;j<l_iAddLength[i];++j )
		{
			l_strIP += "0";
		}
		l_strIP += l_strStripIP[i];
	}
	return l_strIP;
}

std::string		ConvertIPBack(const char*e_strIP)
{
	std::string	l_strIP;
	for( int i=0;i<4;++i )
	{
		int	l_iStartIndex = i*3;
		char l_strCurrentIP[3] = {e_strIP[l_iStartIndex],e_strIP[l_iStartIndex+1],e_strIP[l_iStartIndex+2]};
		int	l_iValue = GetInt(l_strCurrentIP);
		l_strIP += ValueToString(l_iValue);
		if( i != 3 )
			l_strIP += ".";
	}
	return l_strIP;
}

#ifdef ANDROID
void	NetWorkConnectPhaseKeyBoardChancel()
{
	g_bKeyBoardChancel = true;
}


void NetWorkConnectPhaseFinishInput(JNIEnv* env,jobject thiz,  jstring e_InputValue)
{
	SAFE_DELETE(g_pstrTargetIP);
	g_pstrTargetIP = new std::string();
	const char *l_str = env->GetStringUTFChars(e_InputValue, 0);
	*g_pstrTargetIP = l_str;
   env->ReleaseStringUTFChars(e_InputValue,l_str);
}

#endif

cNetWorkConnectPhase::cNetWorkConnectPhase()
{
	m_bDoPlayerDisconnect = false;
	g_pNetWorkConnectPhase = this;
	m_iPlayerIDIndex = 79979;
	m_pBackButon = 0;
	m_pWaitConnectMPDI = 0;
	//std::string	l_strConvertIP = ConvertIP("127.0.0.1");
	//std::string	l_strOriginalIP = ConvertIPBack(l_strConvertIP.c_str());
	this->SetName(NET_WORK_CONNECT_PHASE);
	m_eConnectStep = eCS_SELECT_SERVER_OR_CLIENT;
	m_strMyIP = "127.0.0.1";
	m_strTargetIP = "";
	m_iTotalPlayer = 0;
	m_bIAmServer = false;
	m_strShowSelectionMesasage = "";
	m_vShowSelectionMesasage = Vector2(320,620);
	m_pCreateModeDialog = 0;
	m_pSelectPlayerCountDialog = 0;
	m_strShowWaitingMesasage = "";
	m_vShowWaitingMesasagePos = Vector2(320,620);
	m_iConnectedPlayerCount = 1;
#ifdef ANDROID
	RegisterNativeFunction("util/NumericEditTextDialog","FinishInput","(Ljava/lang/String;)V",(void*)NetWorkConnectPhaseFinishInput);
	RegisterNativeFunction("util/NumericEditTextDialog","Chancel","()V",(void*)NetWorkConnectPhaseKeyBoardChancel);
#endif
}

cNetWorkConnectPhase::~cNetWorkConnectPhase()
{
	SAFE_DELETE(g_pstrTargetIP);
	Destroy();
}

void	cNetWorkConnectPhase::Destroy()
{
	SAFE_DELETE(g_pstrTargetIP);
	SAFE_DELETE(m_pCreateModeDialog);
	SAFE_DELETE(m_pSelectPlayerCountDialog);
	SAFE_DELETE(m_pBackButon);
	cBluffingGirlApp::m_spGameNetwork->Destroy();
	cBluffingGirlApp::m_spGameNetwork->SetNewClientConnectedCallBack(0);
	cBluffingGirlApp::m_spGameNetwork->SetDisconnectCallbackl(0);
}

bool	cNetWorkConnectPhase::GetIP()
{
#ifdef ANDROID
	jclass l_Class = GetCustomJavaClass("util/NetworkUtil");
	EXCEPTION_RETURN(cGameApp::m_spJNIUtilData->pJNIEnv);
	jmethodID l_Method = cGameApp::m_spJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"getLocalAddress","()Ljava/lang/String;");
	EXCEPTION_RETURN(cGameApp::m_spJNIUtilData->pJNIEnv);
	jstring l_strIP =  (jstring)cGameApp::m_spJNIUtilData->pJNIEnv->CallStaticObjectMethod(l_Class, l_Method);
	EXCEPTION_RETURN(cGameApp::m_spJNIUtilData->pJNIEnv);
	if( l_strIP )
	{
		const char *l_strIPChar = cGameApp::m_spJNIUtilData->pJNIEnv->GetStringUTFChars(l_strIP, 0);
		m_strMyIP = ConvertIP(l_strIPChar);
		cGameApp::m_spJNIUtilData->pJNIEnv->ReleaseStringUTFChars(l_strIP,l_strIPChar);
	}
	cGameApp::m_spJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
#else
	cBluffingGirlApp::m_spGameNetwork->GetIP();
	m_strMyIP = ConvertIP(cBluffingGirlApp::m_spGameNetwork->GetIPString().c_str());
#endif
	return true;
}

void	cNetWorkConnectPhase::WakeIPInput()
{
	WCHAR l_strTitle[] = {35531,36664,20837,L'I',L'P',0};
	WCHAR l_strMessage[] = {36899,32218,21040,L's',L'e',L'r',L'v',L'e',L'r',0};
	cGameApp::OpenKeyBoard(1,l_strTitle,l_strMessage);
}

void	cNetWorkConnectPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ServerData")
	{
		ProcessServerData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ConnectTypeData")
	{
		ProcessConnectTypeData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("Button")
	{
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		this->m_pBackButon = cImageButton::GetMe(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("UI")
	{
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		m_pWaitConnectMPDI = cMPDI::GetMe(e_pTiXmlElement,false);
		if( m_pWaitConnectMPDI )
		{
			m_pWaitConnectMPDI->SetAnimationLoop(true);
			m_pWaitConnectMPDI->Init();
		}
	} 
}

void	cNetWorkConnectPhase::ProcessConnectTypeData(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	m_pCreateModeDialog = cButtonDialog::GetMe(e_pTiXmlElement);
}

void	cNetWorkConnectPhase::ProcessServerData(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	m_pSelectPlayerCountDialog = cButtonDialog::GetMe(e_pTiXmlElement);
}


void	cNetWorkConnectPhase::ProcessUIData(TiXmlElement*e_pTiXmlElement)
{

}

void	cNetWorkConnectPhase::Init()
{
	m_pWaitConnectMPDI = 0;
	Destroy();
	cBluffingGirlApp::m_spGameNetwork->Init();
	m_eConnectStep = eCS_SELECT_SERVER_OR_CLIENT;
	PARSE_FAILED_MESSAGE_BOX(this,"BluffingGirl/PhaseData/NetWorkConnectPhase.xml");
	m_iTotalPlayer = 0;
	m_iConnectedPlayerCount = 1;
	m_iPlayerIDIndex = -1;
	if( m_pSelectPlayerCountDialog )
		m_pSelectPlayerCountDialog->Init();
	if( m_pCreateModeDialog )
		m_pCreateModeDialog->Init();
	cBluffingGirlApp::m_spGameNetwork->SetNewClientConnectedCallBack(NewPlayerAdded);
	cBluffingGirlApp::m_spGameNetwork->SetDisconnectCallbackl(PlayerLeave);
	//m_strMyIP = "127.0.0.1";
	//m_strTargetIP = "";
	//m_bIAmServer = false;
	//m_strShowSelectionMesasage = "";
	//m_vShowSelectionMesasage = Vector2(320,620);
	//m_pClientButton = 0;
	//m_pServerButton = 0;
	//m_strShowWaitingMesasage = "";
	//m_vShowWaitingMesasagePos = Vector2(320,620);
}

void	cNetWorkConnectPhase::ProcessMessage(float e_fElpaseTime)
{
	if(cBluffingGirlApp::m_spGameNetwork->IsConnectionFailed())
	{
		m_pBackButon->SetSatisfiedCondition(true);
		this->m_bSatisfiedCondition = true;
		return;
	}
	cBluffingGirlApp::m_spGameNetwork->Update(e_fElpaseTime);
	cFUSynchronizedHold hold(&cBluffingGirlApp::m_spGameNetwork->m_ReceivedData);
	while(cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.size()>0)
	{
		sReceivedPacket*l_pReceivedPacket = cBluffingGirlApp::m_spGameNetwork->m_ReceivedData[0];
		int*l_pppp = (int*)l_pReceivedPacket->pData;
		sGamePacket*l_pGamePacket = (sGamePacket*)l_pReceivedPacket->pData;
		if(l_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_START_ID )
		{
			this->m_iPlayerIDIndex = l_pGamePacket->GetPlayerIndex();
			this->m_iTotalPlayer = l_pGamePacket->GetTotalPlayer();
		}
		else
		if(l_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_PLAYER_COUNT )
		{
			m_iConnectedPlayerCount = l_pGamePacket->GetNumPlayerConnected();
			int	l_iDiecCount = g_pNetWorkConnectPhase->m_iTotalPlayer*DICE_COUNT;
			cBluffingGirlApp::m_spGameNetwork->m_DiceTable.clear();
			char*l_pDiceTable = l_pGamePacket->GetDiceTable();
			for( int i=0;i<l_iDiecCount;++i )
			{
				cBluffingGirlApp::m_spGameNetwork->m_DiceTable.push_back(l_pDiceTable[i]);
			}
		}
		cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.erase(cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.begin());
		delete l_pReceivedPacket;
	}
}

void	cNetWorkConnectPhase::Update(float e_fElpaseTime)
{
	DoPlayerDisconnect();
	ProcessMessage(e_fElpaseTime);
	switch( m_eConnectStep )
	{
		case eCS_SELECT_SERVER_OR_CLIENT:
			if( m_pCreateModeDialog )
			{
				m_pCreateModeDialog->Update(e_fElpaseTime);
				if(m_pCreateModeDialog->IsSatisfiedCondition())
				{
					if( m_pCreateModeDialog->GetResult() == eYNDR_YES )
					{
						GetIP();
						cBluffingGirlApp::m_spGameNetwork->ConnectionCreated(true,m_strMyIP.c_str());
						m_eConnectStep = eCS_SELECT_PLAYER_COUNT;
						m_iPlayerIDIndex = 0;					
					}
					else
					{
						WakeIPInput();
						m_eConnectStep = eCS_INPUT_SERVER_IP;
						cBluffingGirlApp::m_spGameNetwork->CloseSocket();					
					}
				}
			}
			break;
		case eCS_SELECT_PLAYER_COUNT:
			if( m_pSelectPlayerCountDialog )
			{
				m_pSelectPlayerCountDialog->Update(e_fElpaseTime);
				if( m_pSelectPlayerCountDialog->IsSatisfiedCondition() )
				{
					this->m_iTotalPlayer = m_pSelectPlayerCountDialog->GetCurrentWorkingObjectIndex()+2;
					cBluffingGirlApp::m_spGameNetwork->m_DiceTable = GenerateDiceTable(m_iTotalPlayer*DICE_COUNT,6,0);
					this->m_iConnectedPlayerCount = 1;
					m_eConnectStep = eCS_WAIT_FOR_CLIENTS;
				}
			}
			break;
		case eCS_WAIT_FOR_CLIENTS:
				if(m_pWaitConnectMPDI)
					m_pWaitConnectMPDI->Update(e_fElpaseTime);
				if( this->m_iTotalPlayer == this->m_iConnectedPlayerCount )
				{
					this->m_bSatisfiedCondition = true;
				}
			break;
		case eCS_INPUT_SERVER_IP:
			if( g_bKeyBoardChancel )
			{
				g_bKeyBoardChancel = false;
				m_eConnectStep = eCS_SELECT_SERVER_OR_CLIENT;
				if( m_pCreateModeDialog )m_pCreateModeDialog->Init();
			}
			else
			if( g_pstrTargetIP )
			{
				//do connect
				std::string	l_strTargetIUP = ConvertIPBack(g_pstrTargetIP->c_str());
				//std::string	l_strTargetIUP = "192.168.0.189";
				SAFE_DELETE(g_pstrTargetIP);
				cBluffingGirlApp::m_spGameNetwork->ConnectionCreated(false,l_strTargetIUP.c_str());
				m_eConnectStep = eCS_CONNECT_TO_SERVER;
			}
			break;
		case eCS_CONNECT_TO_SERVER:
			if( cBluffingGirlApp::m_spGameNetwork->IsConnected() )
			{
				m_eConnectStep = eCS_WAIT_FOR_CLIENTS;
			}
			else
			if(cBluffingGirlApp::m_spGameNetwork->IsConnectionFailed())
			{
				m_eConnectStep = eCS_NETWORK_ERROR;
				cBluffingGirlApp::m_spGameNetwork->Destroy();
			}
			break;
		case eCS_GO_TO_NEXT_PHASE:
			//g_pNetWorkConnectPhase->SetPlayerIDIndex(m_iPlayerIDIndex,c)			
			break;
		case eCS_NETWORK_ERROR:
			break;
	}
	if( m_pBackButon )
	{
		m_pBackButon->Update(e_fElpaseTime);
		if( m_pBackButon->IsSatisfiedCondition() )
		{
			this->m_bSatisfiedCondition = true;
			cBluffingGirlApp::m_spGameNetwork->Destroy();
		}
	}
}

void	cNetWorkConnectPhase::Render()
{
	switch( m_eConnectStep )
	{
		case eCS_SELECT_SERVER_OR_CLIENT:
				if(m_pCreateModeDialog)
					m_pCreateModeDialog->Render();
			break;
		case eCS_SELECT_PLAYER_COUNT:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			if( m_pSelectPlayerCountDialog )
				m_pSelectPlayerCountDialog->Render();
			break;
		case eCS_WAIT_FOR_CLIENTS:
			{
				cGameApp::m_spGlyphFontRender->SetScale(2);
				if( m_pWaitConnectMPDI )
					m_pWaitConnectMPDI->Render();
				if( this->m_iPlayerIDIndex == 0 )
					cGameApp::RenderFont(130,500,UT::CharToWchar(m_strMyIP));
				else
					cGameApp::RenderFont(130,500,UT::CharToWchar(m_strTargetIP));
				cGameApp::m_spGlyphFontRender->SetScale(3);
				std::wstring	l_strNumPlayerConnect = ValueToStringW(this->m_iTotalPlayer);
				cGameApp::RenderFont(415,645,l_strNumPlayerConnect);
				l_strNumPlayerConnect = ValueToStringW(this->m_iConnectedPlayerCount);
				cGameApp::RenderFont(415,775,l_strNumPlayerConnect);
				cGameApp::m_spGlyphFontRender->SetScale(1);
				//l_strNumPlayerConnect = L"YOU ARE Player";
				//l_strNumPlayerConnect += ValueToStringW(m_iPlayerIDIndex);
				//cGameApp::RenderFont(230,400,l_strNumPlayerConnect);
			}
			break;
		case eCS_INPUT_SERVER_IP:
#ifdef WIN32
			cGameApp::RenderFont(200,200,L"please input server IP");
			cGameApp::RenderFont(200,300,ValueToStringW(m_strTargetIP));
			
#endif
			break;
		case eCS_GO_TO_NEXT_PHASE:
			break;
		case eCS_CONNECT_TO_SERVER:
		{
			std::wstring	l_str = L"Connect to server";
			int	l_iRandOmValue = rand()%10;
			for(int i=0;i<l_iRandOmValue;++i)
				l_str += L".";
			cGameApp::RenderFont(200,300,l_str);
		}
			break;
		case eCS_NETWORK_ERROR:
			cGameApp::RenderFont(200,300,L"Network Error!");
			break;
	}
	if( m_pBackButon )
		m_pBackButon->Render();
}
void	cNetWorkConnectPhase::DebugRender()
{

}
void	cNetWorkConnectPhase::MouseDown(int e_iX,int e_iY)
{
	switch( m_eConnectStep )
	{
		case eCS_SELECT_SERVER_OR_CLIENT:
			if(m_pCreateModeDialog)
				m_pCreateModeDialog->MouseDown(e_iX,e_iY);
			break;
		case eCS_SELECT_PLAYER_COUNT:
			if( m_pSelectPlayerCountDialog )
				m_pSelectPlayerCountDialog->MouseDown(e_iX,e_iY);
			break;
		case eCS_WAIT_FOR_CLIENTS:
			break;
		case eCS_INPUT_SERVER_IP:
			break;
		case eCS_GO_TO_NEXT_PHASE:
			break;
		default:
			break;
	}
	if( m_pBackButon )
		m_pBackButon->MouseDown(e_iX,e_iY);
}
void	cNetWorkConnectPhase::MouseUp(int e_iX,int e_iY)
{
	switch( m_eConnectStep )
	{
		case eCS_SELECT_SERVER_OR_CLIENT:
			if(m_pCreateModeDialog)
				m_pCreateModeDialog->MouseUp(e_iX,e_iY);
			break;
		case eCS_SELECT_PLAYER_COUNT:
			if( m_pSelectPlayerCountDialog )
				m_pSelectPlayerCountDialog->MouseUp(e_iX,e_iY);
			break;
		case eCS_WAIT_FOR_CLIENTS:
			break;
		case eCS_INPUT_SERVER_IP:
			break;
		case eCS_GO_TO_NEXT_PHASE:
			break;
		case eCS_NETWORK_ERROR:
			this->m_bSatisfiedCondition = true;
			break;
		default:
			break;
	}
	if( m_pBackButon )
		m_pBackButon->MouseUp(e_iX,e_iY);
}

void	cNetWorkConnectPhase::MouseMove(int e_iX,int e_iY)
{
	switch( m_eConnectStep )
	{
		case eCS_SELECT_SERVER_OR_CLIENT:
			if(m_pCreateModeDialog)
				m_pCreateModeDialog->MouseMove(e_iX,e_iY);
			break;
		case eCS_SELECT_PLAYER_COUNT:
			if( m_pSelectPlayerCountDialog )
				m_pSelectPlayerCountDialog->MouseMove(e_iX,e_iY);
			break;
		case eCS_WAIT_FOR_CLIENTS:
			break;
		case eCS_INPUT_SERVER_IP:
			break;
		case eCS_GO_TO_NEXT_PHASE:
			break;
		default:
			break;
	}
	if( m_pBackButon )
		m_pBackButon->MouseMove(e_iX,e_iY);
}
const	WCHAR*	cNetWorkConnectPhase::GetNextPhaseName()
{
	if( m_pBackButon && m_pBackButon->IsSatisfiedCondition() )
	{
		return TITLE_PHASE_NAME;
	}
	if( m_iTotalPlayer != m_iConnectedPlayerCount )
		return TITLE_PHASE_NAME;
	return BLUFFING_MULTI_PLAY_DICEGAME_PHASE_NAME;
}

void	cNetWorkConnectPhase::KeyUp(char e_cKey)
{
#ifdef WIN32
	if( e_cKey == 8 )
	{
		if( m_strTargetIP.length() == 0 )
		{
			g_bKeyBoardChancel = true;
			return;
		}
		char	l_str[MAX_PATH];
		sprintf(l_str,"%s",m_strTargetIP.c_str());
		l_str[strlen(l_str)-1] = 0;
		m_strTargetIP = l_str;
	}
	else
	if( e_cKey == 13 )
	{
		SAFE_DELETE(g_pstrTargetIP);
		g_pstrTargetIP = new std::string;
		*g_pstrTargetIP = m_strTargetIP;
	}
	else
		m_strTargetIP += e_cKey;
#endif
}

void	cNetWorkConnectPhase::KeyDown(char e_cKey)
{

}

void	cNetWorkConnectPhase::KeyPress(char e_cKey)
{

}

void*	cNetWorkConnectPhase::GetData()
{
	return (void*)this;
}

void	cNetWorkConnectPhase::DoPlayerDisconnect()
{
	if( m_bDoPlayerDisconnect )
	{
		sGamePacket	l_GamePacket;
		int	l_iNumPlayer = cBluffingGirlApp::m_spGameNetwork->GetNumPlayerConnected();
		l_GamePacket.SetMessageID(eGMI_TO_CLIENT_PLAYER_COUNT);
		l_GamePacket.SetNumPlayerConnected(l_iNumPlayer);;
		sSendPacket l_Packet;
		l_Packet.pData = (char*)&l_GamePacket;
		l_Packet.iSize = sizeof(int)+sizeof(int);
		cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_Packet);
		//if I am clientmserver is close connection.
		l_Packet.pData = 0;
		m_bDoPlayerDisconnect = false;
	}
}

void Thread()
{
	//OpenSocket();
	//Status = TryToConnect;
}