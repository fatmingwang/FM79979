#include "stdafx.h"
#include "Bluetooth.h"

#pragma comment(lib, "bthprops")
#pragma comment(lib,"setupapi.lib")
#pragma comment(lib, "user32")
#pragma comment(lib,"Ws2_32.lib")


extern ULONG NameToBthAddr(const LPWSTR pszRemoteName,PSOCKADDR_BTH pRemoteBtAddr);
extern int ChangeLocalBluetoothRadioName(const wchar_t*e_strBlueToothName);



TYPDE_DEFINE_MARCO(cBluetoothSinglton)
sBluetoothPacket::sBluetoothPacket()
{
	pData = nullptr;
	iSize = 0;
}

sBluetoothPacket::~sBluetoothPacket()
{
	SAFE_DELETE(pData);
}

cBluetoothSinglton::cBluetoothSinglton()
{
	m_LocalSocket = INVALID_SOCKET;
	m_bLostConnection = false;
	//https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recv
	WSADATA     WSAData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &WSAData);
}

cBluetoothSinglton::~cBluetoothSinglton()
{
	Disconnect();
	WSACleanup();
}

void cBluetoothSinglton::Init()
{
	Disconnect();
	bool            l_bRetCode = true;
	char *          pszDataBuffer = NULL;
	WSAQUERYSET     wsaQuerySet = { 0 };
	SOCKADDR_BTH    SockAddrBthLocal = { 0 };
	//
	// Open a bluetooth socket using RFCOMM protocol
	//
	m_LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
}

void cBluetoothSinglton::Update(float e_fElpaseTime)
{
	if (m_LocalSocket == INVALID_SOCKET)
	{//do somethinig to reconnect?
		//this->Init();
	}
}

bool cBluetoothSinglton::CreateAsServer(const wchar_t*e_strServerName)
{
	ChangeLocalBluetoothRadioName(e_strServerName);
	if (INVALID_SOCKET != m_LocalSocket)
	{
		bool            l_bRetCode = true;
		char *          pszDataBuffer = NULL;
		WSAQUERYSET     wsaQuerySet = { 0 };
		SOCKADDR_BTH    SockAddrBthLocal = { 0 };
		if (l_bRetCode)
		{
			//
			// Setting address family to AF_BTH indicates winsock2 to use Bluetooth port
			//
			SockAddrBthLocal.addressFamily = AF_BTH;
			SockAddrBthLocal.port = BT_PORT_ANY;
			//
			// bind() associates a local address and port combination
			// with the socket just created. This is most useful when
			// the application is a server that has a well-known port
			// that clients know about in advance.
			//
			//add : for make it not as std::bind
			if (::bind(m_LocalSocket, (struct sockaddr *) &SockAddrBthLocal, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
			{
				auto l_iResult = WSAGetLastError();
				closesocket(m_LocalSocket);
				//wprintf(L"=CRITICAL= | bind() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
				l_bRetCode = false;
			}
		}
		if (l_bRetCode)
		{
			int iAddrLen = sizeof(SOCKADDR_BTH);
			if (SOCKET_ERROR == getsockname(m_LocalSocket, (struct sockaddr *)&SockAddrBthLocal, &iAddrLen))
			{
				//wprintf(L"=CRITICAL= | getsockname() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
				l_bRetCode = false;
			}
		}

		if (l_bRetCode)
		{
			//
			// CSADDR_INFO
			//
			_CSADDR_INFO   l_CSAddrInfo;
			l_CSAddrInfo.LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
			l_CSAddrInfo.LocalAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
			l_CSAddrInfo.RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
			l_CSAddrInfo.RemoteAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
			l_CSAddrInfo.iSocketType = SOCK_STREAM;
			l_CSAddrInfo.iProtocol = BTHPROTO_RFCOMM;
			//
			// If we got an address, go ahead and advertise it.
			//
			ZeroMemory(&wsaQuerySet, sizeof(WSAQUERYSET));
			wsaQuerySet.dwSize = sizeof(WSAQUERYSET);
			wsaQuerySet.lpServiceClassId = (LPGUID)&GUID_DEVCLASS_BLUETOOTH;
			wsaQuerySet.lpszServiceInstanceName = L"FM";
			wsaQuerySet.lpszComment = L"TEST";
			wsaQuerySet.dwNameSpace = NS_BTH;
			wsaQuerySet.dwNumberOfCsAddrs = 1;      // Must be 1.
			wsaQuerySet.lpcsaBuffer = &l_CSAddrInfo; // Req'd.

													 //
													 // As long as we use a blocking accept(), we will have a race
													 // between advertising the service and actually being ready to
													 // accept connections.  If we use non-blocking accept, advertise
													 // the service after accept has been called.
													 //
													 //https://stackoverflow.com/questions/19393407/how-do-i-find-the-port-name-for-a-bluetooth-device-with-a-specific-device-name
			int l_iResult2 = WSAGetLastError();
			if (SOCKET_ERROR == WSASetService(&wsaQuerySet, RNRSERVICE_REGISTER, 0)) {//SERVICE_MULTIPLE
				int l_iResult = WSAGetLastError();
				wprintf(L"=CRITICAL= | WSASetService() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
				l_bRetCode = false;
			}
		}

		//
		// listen() call indicates winsock2 to listen on a given socket for any incoming connection.
		//
		if (l_bRetCode)
		{
			//https://stackoverflow.com/questions/10002868/what-value-of-backlog-should-i-use
			if (SOCKET_ERROR == listen(m_LocalSocket, 4))
			{
				wprintf(L"=CRITICAL= | listen() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
				l_bRetCode = false;
			}
		}
		if (!l_bRetCode)
		{
			if (INVALID_SOCKET != m_LocalSocket)
			{
				closesocket(m_LocalSocket);
				m_LocalSocket = INVALID_SOCKET;
			}
		}
		else
		{
			f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cBluetoothSinglton::ServerUpdate, this, std::placeholders::_1);
			ThreadDetach(l_f_ThreadWorkingFunction);
			return true;
		}
	}
	return false;
}

bool cBluetoothSinglton::CreateAsClient(const wchar_t*e_strServerName)
{
	if (INVALID_SOCKET != m_LocalSocket)
	{
		SOCKADDR_BTH RemoteBthAddr = { 0 };
		if (NameToBthAddr((const LPWSTR)e_strServerName, &RemoteBthAddr) == 0)
		{
			RemoteBthAddr.addressFamily = AF_BTH;
			RemoteBthAddr.serviceClassId = GUID_DEVCLASS_BLUETOOTH;
			RemoteBthAddr.port = 0;
			if (connect(m_LocalSocket,(struct sockaddr *) &RemoteBthAddr,sizeof(SOCKADDR_BTH)) != SOCKET_ERROR)
			{
				return true;
			}
			//wprintf(L"=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		}
	}
	return false;
}

void cBluetoothSinglton::Disconnect()
{
	if (m_LocalSocket != INVALID_SOCKET)
	{
		this->m_bLeaveThread = true;
		CloseThreadAndWaitUntilFinish();
		size_t l_uiSize = m_ConnectedSocketVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if (SOCKET_ERROR == closesocket(m_ConnectedSocketVector[i]))
			{
				wprintf(L"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
			}
		}
		closesocket(m_LocalSocket);
		m_LocalSocket = INVALID_SOCKET;
	}
	DELETE_VECTOR(m_BluetoothPacketVector);
}

bool cBluetoothSinglton::SetDeviceName(const wchar_t * e_strName)
{
	return ChangeLocalBluetoothRadioName(e_strName) == 0 ? true : false;
}

void cBluetoothSinglton::ServerUpdate(float e_fElpaseTime)
{
	int             iCxnCount = 0;
	while(!this->m_bLeaveThread)
	{
		//
		// accept() call indicates winsock2 to wait for any
		// incoming connection request from a remote socket.
		// If there are already some connection requests on the queue,
		// then accept() extracts the first request and creates a new socket and
		// returns the handle to this newly created socket. This newly created
		// socket represents the actual connection that connects the two sockets.
		//
		auto l_pClientSocket = accept(m_LocalSocket, NULL, NULL);
		if (INVALID_SOCKET != l_pClientSocket)
		{
			cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
			m_ConnectedSocketVector.push_back(l_pClientSocket);
		}
		//
		// Read data from the incoming stream
		//
		size_t l_uiSize = m_ConnectedSocketVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			auto l_pSocket = m_ConnectedSocketVector[i];
			char pszDataBuffer[TEMP_SIZE];
			memset(pszDataBuffer, 0, sizeof(pszDataBuffer));
			//
			// recv() call indicates winsock2 to receive data
			// of an expected length over a given connection.
			// recv() may not be able to get the entire length
			// of data at once.  In such case the return value,
			// which specifies the number of bytes received,
			// can be used to calculate how much more data is
			// pending and accordingly recv() can be called again.
			//
			UINT            iLengthReceived = 0;
			iLengthReceived = recv(l_pSocket, pszDataBuffer, TEMP_SIZE, 0);
			switch (iLengthReceived)
			{
				case 0: // socket connection has been closed gracefully
				{
					closesocket(l_pSocket);
					cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
					m_ConnectedSocketVector.erase(m_ConnectedSocketVector.begin() + i);
					--i;
				}
				break;
				case SOCKET_ERROR:
					//lost connection?
					goto FAILED;
				break;
				default:
				{
					sBluetoothPacket*l_pBluetoothPacket = new sBluetoothPacket();
					l_pBluetoothPacket->iSize = iLengthReceived;
					memcpy(l_pBluetoothPacket->pData, pszDataBuffer, iLengthReceived);
					cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
					m_BluetoothPacketVector.push_back(l_pBluetoothPacket);
				}
				break;
			}
		}
	}
FAILED:
	//
	size_t l_uiSize = m_ConnectedSocketVector.size();
	for (size_t i = 0; i < l_uiSize; i++)
	{
		if (SOCKET_ERROR == closesocket(m_ConnectedSocketVector[i]))
		{
			wprintf(L"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
		}
	}
	if (INVALID_SOCKET != m_LocalSocket)
	{
		closesocket(m_LocalSocket);
		m_LocalSocket = INVALID_SOCKET;
	}
}

void cBluetoothSinglton::ClientUpdate(float e_fElpaseTime)
{
	while (!this->m_bLeaveThread)
	{
		char pszDataBuffer[TEMP_SIZE];
		memset(pszDataBuffer, 0, sizeof(pszDataBuffer));
		UINT            iLengthReceived = 0;
		iLengthReceived = recv(m_LocalSocket, pszDataBuffer, TEMP_SIZE, 0);
		switch (iLengthReceived)
		{
		case 0: // socket connection has been closed gracefully
		case SOCKET_ERROR:
			//lost connection?
			goto FAILED;
			break;
		default:
		{
			sBluetoothPacket*l_pBluetoothPacket = new sBluetoothPacket();
			l_pBluetoothPacket->iSize = iLengthReceived;
			memcpy(l_pBluetoothPacket->pData, pszDataBuffer, iLengthReceived);
			cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
			m_BluetoothPacketVector.push_back(l_pBluetoothPacket);
		}
		break;
		}
	}
FAILED:
	closesocket(this->m_LocalSocket);
	m_LocalSocket = INVALID_SOCKET;
}

void cBluetoothSinglton::ConnectToServerUpdate(float e_fElpaseTime)
{
	//SOCKADDR_BTH RemoteBthAddr = { 0 };
	//if (NameToBthAddr((const LPWSTR)e_strServerName, &RemoteBthAddr) == 0)
	//{
	//	SOCKADDR_BTH    SockAddrBthServer
	//		return true;
	//}
	//return false;
}

void cBluetoothSinglton::GetReceivedData(std::vector<sBluetoothPacket*>* e_pDataVector)
{
	if (e_pDataVector)
	{
		cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
		size_t l_uiSize = m_BluetoothPacketVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			e_pDataVector->push_back(m_BluetoothPacketVector[i]);
		}
		m_BluetoothPacketVector.clear();
	}
}

void cBluetoothSinglton::FindDevices()
{
}

void cBluetoothSinglton::SendDataToAllClient(int e_iLength, char * e_pData)
{
	cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
	size_t l_uiSize = this->m_ConnectedSocketVector.size();
	for (size_t i = 0; i < l_uiSize; i++)
	{
		auto l_pSocket = m_ConnectedSocketVector[i];
		if (SOCKET_ERROR == send(l_pSocket, e_pData, e_iLength,0))
		{
			//wprintf(L"=CRITICAL= | send() call failed w/socket = [0x%I64X], szData = [%p], dataLen = [%I64u]. WSAGetLastError=[%d]\n", (ULONG64)LocalSocket, pszData, (ULONG64)CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
			//break;
			continue;
		}
	}
}

void cBluetoothSinglton::SendDataToServer(int e_iLength, char * e_pData)
{
	if (m_LocalSocket != INVALID_SOCKET)
	{
		if (SOCKET_ERROR == send(m_LocalSocket, e_pData, e_iLength,0))
		{
			//do reconnect?
			//lost connect and do reconnect?
			//	wprintf(L"=CRITICAL= | send() call failed w/socket = [0x%I64X], szData = [%p], dataLen = [%I64u]. WSAGetLastError=[%d]\n", (ULONG64)LocalSocket, pszData, (ULONG64)CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
			//ulRetCode = CXN_ERROR;
			//break;
		}
	}
}
