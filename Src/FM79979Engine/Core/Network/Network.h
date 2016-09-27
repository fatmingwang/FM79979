#ifndef	_NET_WORK_H_
#define _NET_WORK_H_

#include "SDL_net.h"
#include "../Synchronization/AllFUThreadInclude.h"
class	cFUThreadPool;

namespace FATMING_CORE
{
	#define	NAME_LENGTH	16
	struct	sSendPacket
	{
		size_t	iSize;
		char*	pData;
		sSendPacket()
		{
			pData = 0;
			iSize = 0;
		}
		~sSendPacket()
		{
			SAFE_DELETE(pData);
		}
	};

	struct	sReceivedPacket
	{
		bool		bDataInlegal;
		char*		pCurrentStoreData;
		int			iRestSizeToReceive;
		int			iSize;
		char*		pData;
		_TCPsocket*	pReceivedSocket;
		bool		bNextDataAlivable;
		sReceivedPacket()
		{
			bDataInlegal = false;
			pReceivedSocket = 0;
			iRestSizeToReceive = 0;
			pData = 0;
			iSize = 0;
		}
		~sReceivedPacket()
		{
			SAFE_DELETE(pData);
		}
		bool	ReceiveData(_TCPsocket*e_pTCPsocket);
		sReceivedPacket*	NextPacket();
	};

	template<class T> class	cVectorDataSynchornized:public cFUSynchronized,public std::vector<T*>
	{
	public:
		void	Clear(){ DELETE_VECTOR((*this),T*); }
		cVectorDataSynchornized(){}
		virtual ~cVectorDataSynchornized(){ Clear(); }
	};

	class	cNetwork
	{
		bool								m_bLeaveThread;
		//
		void								AddClient(_TCPsocket*e__pTCPsocket);
		bool								RemoveClient(_TCPsocket*e__pTCPsocket);
		//
		bool								m_bDisconnectCall;
	protected:
		//
		int									m_iPort;
		IPaddress							m_IP;
		unsigned char						m_MyIP[4];
		string								m_strIP;
		std::string							m_strHost;
		//
		bool								m_bConnectionFailed;
		friend void							NetworkListenThread(size_t _workParameter, size_t _pUri);
		friend void							NetworkListenThreadDone(size_t _workParameter, size_t _pUri);
		cFUThreadPool*						m_pThreadPool;
		//own socket for server or client
		_TCPsocket*							m_pSocket;
		/* create a socket set that has the server socket and all the client sockets */
		_SDLNet_SocketSet*					m_pAllSocketToListenClientMessage;		
		bool								CreateSocksetToListenData();
		cVectorDataSynchornized<_TCPsocket>	m_ClientSocketVector;
		//if e_strIP is nullptr it's server
		bool								OpenSocket(int e_iPort,const char*e_strIP);
		bool								CreateThread();
		//_TCPsocket* will be sent.,!!!do not call any SendDataToServer Or SendDataToClient in m_pDisconnectCallback and m_pNewClientConnectedCallBack,the m_ClientSocketVector will be locked!.
		GET_SET_DEC(DoButtonGoal_Callback,m_pDisconnectCallback,GetDisconnectCallback,SetDisconnectCallbackl);
		GET_SET_DEC(DoButtonGoal_Callback,m_pNewClientConnectedCallBack,GetNewClientConnectedCallBack,SetNewClientConnectedCallBack);
		std::map<_TCPsocket*,sReceivedPacket*>m_TempReceiveData;
	public:
		cNetwork();
		virtual ~cNetwork();
		//while data was recevied,it will store at m_ReceviedData,delete it after u use it.
		//method to use this:
		//cFUSynchronizedHold hold(&m_ReceivedData);
		cVectorDataSynchornized<sReceivedPacket>	m_ReceivedData;
		//
		bool								IsConnectionFailed();
		bool								IsConnected();
		bool								IsReceivedData();

		bool								SendData(_TCPsocket*e_pTCPsocket,sSendPacket*e_pPacket);
		bool								SendDataToServer(sSendPacket*e_pPacket);
		bool								SendDataToAllClient(sSendPacket*e_pPacket);

		bool								CreateAsServer(int e_iPort);
		bool								CreateAsClient(int e_iPort,const char*e_strIP);

		void								Destroy();
		void								CloseSocket();

		bool								GetIP();
		void								RemoveAllClient();
		size_t								ClientCount(){ return m_ClientSocketVector.size(); }

		std::string	GetIPString()			{return m_strIP;}
		void								Init();
		virtual	void						Update(float e_fElpaseTime);
		//cNetwork::DownloadFile(L"http://www.khronos.org/files/collada_cts_rfq_2012.pdf",L"79979.pdf");
		//http://stackoverflow.com/questions/5291432/waiting-for-urldownloadtofile-to-end
		static		bool					DownloadFile(const wchar_t*e_strURL,const wchar_t*e_strLocalFileName);
	};
//end namespace
}
#endif