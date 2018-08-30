#ifdef WIN32
#include <winsock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <Setupapi.h>
#include "../stdafx.h"
#include "Bluetooth.h"
#include "../GameplayUT/GameApp.h"
#pragma comment(lib, "bthprops")
#pragma comment(lib,"setupapi.lib")
#pragma comment(lib, "user32")
#pragma comment(lib,"Ws2_32.lib")

namespace FATMING_CORE
{

	extern ULONG NameToBthAddr(const LPWSTR pszRemoteName, PSOCKADDR_BTH pRemoteBtAddr);
	extern int ChangeLocalBluetoothRadioName(const wchar_t*e_strBlueToothName);

	int Bluetooth_SDLNet_CheckSockets(SDLNet_SocketSet set, unsigned int timeout)
	{
		int i;
		SOCKET maxfd;
		int retval;
		struct timeval tv;
		fd_set mask;

		/* Find the largest file descriptor */
		maxfd = 0;
		for (i = set->numsockets - 1; i >= 0; --i)
		{
			if (set->sockets[i]->channel > maxfd)
			{
				maxfd = set->sockets[i]->channel;
			}
		}

		/* Check the file descriptors for available data */
		do
		{
			SDLNet_SetLastError(0);

			/* Set up the mask of file descriptors */
			FD_ZERO(&mask);
			for (i = set->numsockets - 1; i >= 0; --i)
			{
				FD_SET(set->sockets[i]->channel, &mask);
			}

			/* Set up the timeout */
			tv.tv_sec = timeout / 1000;
			tv.tv_usec = (timeout % 1000) * 1000;

			// Look!//until message is coming or it will always stock here,so this function mu call at multithread
			retval = select((int)maxfd + 1, &mask, NULL, NULL, &tv);
		} while (SDLNet_GetLastError() == EINTR);

		/* Mark all file descriptors ready that have data available */
		if (retval > 0)
		{
			for (i = set->numsockets - 1; i >= 0; --i)
			{
				if (FD_ISSET(set->sockets[i]->channel, &mask))
				{
					set->sockets[i]->ready = 1;
				}
			}
		}
		return(retval);
	}


	int BluetoothSinglton_SDLNet_AddSocket(SDLNet_SocketSet set, SDLNet_Socket* sock)
	{
		if (sock != NULL)
		{
			if (set->numsockets == set->maxsockets)
			{
				//SDLNet_SetError("socketset is full");
				return(-1);
			}
			set->sockets[set->numsockets++] = sock;
		}
		return(set->numsockets);
	}

	TYPDE_DEFINE_MARCO(cBluetoothSinglton)
		sBluetoothPacket::sBluetoothPacket(int e_iSize, char*e_pData)
	{
		pData = new char[e_iSize];
		iSize = e_iSize;
		memcpy(pData, e_pData, e_iSize);
	}

	sBluetoothPacket::~sBluetoothPacket()
	{
		SAFE_DELETE(pData);
	}

	cBluetoothSinglton::cBluetoothSinglton()
	{
		m_ClientConnectionLostFunction = nullptr;
		m_ClientConnectionAddFunction = nullptr;//windows only
		m_ConnectionLostFunction = nullptr;
		m_ConnectionConnectedFunction = nullptr;
		m_pAllSocketToListenClientMessage = nullptr;
		m_LocalSocket.channel = INVALID_SOCKET;
		m_LocalSocket.ready = false;
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

	void	cBluetoothSinglton::BluetoothStatusMPDIResourceCreate()
	{
		for (size_t i = 0; i < eBTCS_MAX; i++)
		{
			m_pBluetoothStatusMPDI[i] = nullptr;
		}
		auto l_pMPDIList = cGameApp::GetMPDIListByFileName(L"Bluetooth/BluetootStatus.mpdi");
		if (l_pMPDIList)
		{
			for (size_t i = 0; i < eBTCS_MAX; i++)
			{
				m_pBluetoothStatusMPDI[i] = l_pMPDIList->GetObject(i);
				if (m_pBluetoothStatusMPDI[i])
					m_pBluetoothStatusMPDI[i]->Init();
			}
		}
	}

	void	cBluetoothSinglton::BluetoothStatusMPDIResourceDelete()
	{
		auto l_pMPDIList = cGameApp::GetMPDIListByFileName(L"Bluetooth/BluetootStatus.mpdi");
		if (l_pMPDIList)
		{
			cGameApp::m_spAnimationParser->RemoveObject(l_pMPDIList);
		}
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
		m_LocalSocket.channel = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		m_LocalSocket.ready = false;
	}

	void cBluetoothSinglton::Update(float e_fElpaseTime)
	{
		if (m_LocalSocket.channel == INVALID_SOCKET)
		{//do somethinig to reconnect?
			//this->Init();
		}
	}

	void	cBluetoothSinglton::Render(int e_iPosX, int e_iPosY)
	{
		auto l_strDescription = g_strConnectionStatus[m_eBluetoothConnectionStatus];
		cGameApp::RenderFont(Vector2(e_iPosX, e_iPosY), l_strDescription);
	}

	bool cBluetoothSinglton::CreateAsServer(const wchar_t*e_strServerName)
	{
		ChangeLocalBluetoothRadioName(e_strServerName);
		if (m_LocalSocket.channel != INVALID_SOCKET)
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
				if (::bind(m_LocalSocket.channel, (struct sockaddr *) &SockAddrBthLocal, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
				{
					auto l_iResult = WSAGetLastError();
					closesocket(m_LocalSocket.channel);
					//wprintf(L"=CRITICAL= | bind() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket.channel, WSAGetLastError());
					l_bRetCode = false;
				}
			}
			if (l_bRetCode)
			{
				int iAddrLen = sizeof(SOCKADDR_BTH);
				if (SOCKET_ERROR == getsockname(m_LocalSocket.channel, (struct sockaddr *)&SockAddrBthLocal, &iAddrLen))
				{
					//wprintf(L"=CRITICAL= | getsockname() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket.channel, WSAGetLastError());
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
				if (SOCKET_ERROR == listen(m_LocalSocket.channel, 4))
				{
					wprintf(L"=CRITICAL= | listen() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket.channel, WSAGetLastError());
					l_bRetCode = false;
				}
			}
			if (!l_bRetCode)
			{
				if (INVALID_SOCKET != m_LocalSocket.channel)
				{
					closesocket(m_LocalSocket.channel);
					m_LocalSocket.channel = INVALID_SOCKET;
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
		if (m_LocalSocket.channel != INVALID_SOCKET)
		{
			SOCKADDR_BTH RemoteBthAddr = { 0 };
			if (NameToBthAddr((const LPWSTR)e_strServerName, &RemoteBthAddr))
			{
				RemoteBthAddr.addressFamily = AF_BTH;
				RemoteBthAddr.serviceClassId = GUID_DEVCLASS_BLUETOOTH;
				RemoteBthAddr.port = 0;
				if (connect(m_LocalSocket.channel, (struct sockaddr *) &RemoteBthAddr, sizeof(SOCKADDR_BTH)) != SOCKET_ERROR)
				{
					if(m_ConnectionConnectedFunction)
						m_ConnectionConnectedFunction(&m_LocalSocket);
					f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cBluetoothSinglton::ClientUpdate, this, std::placeholders::_1);
					ThreadDetach(l_f_ThreadWorkingFunction);
					return true;
				}
				//wprintf(L"=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
			}
		}
		return false;
	}

	void cBluetoothSinglton::Disconnect()
	{
		if (m_LocalSocket.channel != INVALID_SOCKET)
		{
			this->m_bLeaveThread = true;
			Sleep(10);
			closesocket(m_LocalSocket.channel);
			CloseThreadAndWaitUntilFinish();
			size_t l_uiSize = m_ConnectedSocketVector.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if (SOCKET_ERROR == closesocket(m_ConnectedSocketVector[i]->channel))
				{
					wprintf(L"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket.channel, WSAGetLastError());
				}
			}
			m_LocalSocket.channel = INVALID_SOCKET;
		}
		DELETE_VECTOR(m_BluetoothPacketVector);
		DELETE_VECTOR(m_ConnectedSocketVector);
	}

	bool cBluetoothSinglton::SetDeviceName(const wchar_t * e_strName)
	{
		return ChangeLocalBluetoothRadioName(e_strName) == 0 ? true : false;
	}

	bool	cBluetoothSinglton::CreateSocksetToListenData()
	{
		//FMLog::LogWithFlag("create socket\n");
		if (m_pAllSocketToListenClientMessage)
			SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
		cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
		int	l_iNumClient = l_iNumClient = (int)m_ConnectedSocketVector.size();
		//+1 fo server
		m_pAllSocketToListenClientMessage = SDLNet_AllocSocketSet(l_iNumClient + 1);
		if (!m_pAllSocketToListenClientMessage)
		{
			//FMLog::LogWithFlag("SDLNet_AllocSocketSet: %s\n", SDL_GetError());
			//exit(1); /*most of the time this is a major error, but do what you want. */
			return false;
		}

		BluetoothSinglton_SDLNet_AddSocket(m_pAllSocketToListenClientMessage, &m_LocalSocket);
		for (int i = 0; i < l_iNumClient; i++)
			BluetoothSinglton_SDLNet_AddSocket(m_pAllSocketToListenClientMessage, m_ConnectedSocketVector[i]);
		return true;
	}

	void cBluetoothSinglton::ServerUpdate(float e_fElpaseTime)
	{
		int             iCxnCount = 0;
		while (!this->m_bLeaveThread)
		{
			if (!CreateSocksetToListenData())
			{
				goto FAILED;
			}
			int l_iNumready = Bluetooth_SDLNet_CheckSockets(m_pAllSocketToListenClientMessage, (UINT)1000);
			if (l_iNumready == -1)
			{
				goto FAILED;
			}
			if (l_iNumready == 0)
				continue;
			if (m_bLeaveThread)
			{
				goto FAILED;
			}
			//
			// accept() call indicates winsock2 to wait for any
			// incoming connection request from a remote socket.
			// If there are already some connection requests on the queue,
			// then accept() extracts the first request and creates a new socket and
			// returns the handle to this newly created socket. This newly created
			// socket represents the actual connection that connects the two sockets.
			//
			if (m_LocalSocket.ready)
			{
				auto l_pClientSocket = accept(m_LocalSocket.channel, NULL, NULL);
				if (INVALID_SOCKET != l_pClientSocket)
				{
					m_LocalSocket.ready = 0;
					cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
					SDLNet_Socket*l_pSDLNet_Socket = new SDLNet_Socket();
					l_pSDLNet_Socket->channel = l_pClientSocket;
					l_pSDLNet_Socket->ready = 0;
					m_ConnectedSocketVector.push_back(l_pSDLNet_Socket);
					if (m_ClientConnectionAddFunction)
						m_ClientConnectionAddFunction(l_pSDLNet_Socket);
				}
			}
			//
			// Read data from the incoming stream
			//
			size_t l_uiSize = m_ConnectedSocketVector.size();
			for (int i = 0; i < l_uiSize; i++)
			{
				auto l_pSocket = m_ConnectedSocketVector[i];
				if (l_pSocket->ready)
				{
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
					iLengthReceived = recv(l_pSocket->channel, pszDataBuffer, TEMP_SIZE, 0);
					l_pSocket->ready = 0;
					switch (iLengthReceived)
					{
					case 0: // socket connection has been closed gracefully
					{
						{
							cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothSocketVectorMutex);
							if (m_ClientConnectionLostFunction)
								m_ClientConnectionLostFunction(l_pSocket);
							closesocket(l_pSocket->channel);
							delete l_pSocket;
							m_ConnectedSocketVector.erase(m_ConnectedSocketVector.begin() + i);
						}
						--i;
						--l_uiSize;
					}
					break;
					case SOCKET_ERROR:
						//lost connection?
						goto FAILED;
						break;
					default:
					{
						sBluetoothPacket*l_pBluetoothPacket = new sBluetoothPacket(iLengthReceived, pszDataBuffer);
						cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
						m_BluetoothPacketVector.push_back(l_pBluetoothPacket);
					}
					break;
					}
				}
			}
		}
	FAILED:
		//
		m_bLeaveThread = true;
		if (m_ConnectionLostFunction)
			m_ConnectionLostFunction(&m_LocalSocket);
		size_t l_uiSize = m_ConnectedSocketVector.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			if (closesocket(m_ConnectedSocketVector[i]->channel) == SOCKET_ERROR)
			{
				wprintf(L"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket.channel, WSAGetLastError());
			}
		}
		if (m_LocalSocket.channel != INVALID_SOCKET)
		{
			closesocket(m_LocalSocket.channel);
			m_LocalSocket.channel = INVALID_SOCKET;
		}
	}

	void cBluetoothSinglton::ClientUpdate(float e_fElpaseTime)
	{
		while (!this->m_bLeaveThread)
		{
			char pszDataBuffer[TEMP_SIZE];
			memset(pszDataBuffer, 0, sizeof(pszDataBuffer));
			UINT            iLengthReceived = recv(m_LocalSocket.channel, pszDataBuffer, TEMP_SIZE, 0);
			switch (iLengthReceived)
			{
			case 0: // socket connection has been closed gracefully
			case SOCKET_ERROR:
				//lost connection?
				goto FAILED;
				break;
			default:
			{
				sBluetoothPacket*l_pBluetoothPacket = new sBluetoothPacket(iLengthReceived, pszDataBuffer);
				cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
				m_BluetoothPacketVector.push_back(l_pBluetoothPacket);
			}
			break;
			}
		}
	FAILED:
		if (m_ConnectionLostFunction)
			m_ConnectionLostFunction(&m_LocalSocket);
		closesocket(this->m_LocalSocket.channel);
		m_LocalSocket.channel = INVALID_SOCKET;
		m_bLeaveThread = true;
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
			if (send(l_pSocket->channel, e_pData, e_iLength, 0) == SOCKET_ERROR)
			{
				//wprintf(L"=CRITICAL= | send() call failed w/socket = [0x%I64X], szData = [%p], dataLen = [%I64u]. WSAGetLastError=[%d]\n", (ULONG64)LocalSocket, pszData, (ULONG64)CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
				//break;
				continue;
			}
		}
	}

	void cBluetoothSinglton::SendDataToServer(int e_iLength, char * e_pData)
	{
		if (m_LocalSocket.channel != INVALID_SOCKET)
		{
			if (SOCKET_ERROR == send(m_LocalSocket.channel, e_pData, e_iLength, 0))
			{
				//do reconnect?
				//lost connect and do reconnect?
				//	wprintf(L"=CRITICAL= | send() call failed w/socket = [0x%I64X], szData = [%p], dataLen = [%I64u]. WSAGetLastError=[%d]\n", (ULONG64)LocalSocket, pszData, (ULONG64)CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
				//ulRetCode = CXN_ERROR;
				//break;
			}
		}
	}
}
#endif