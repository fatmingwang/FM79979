#ifndef _GAME_NET_WORK_
#define	_GAME_NET_WORK_

//struct sockaddr_in sa;
//// store this IP address in sa:
//inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));
//
//// now get it back and print it
//inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
enum	eGameMessageID
{
	//server	
	eGMI_TO_SERVER_I_AM_READY = 79979,//into game phase and ready to start game.
	eGMI_TO_SERVER_CALL,
	eGMI_TO_SERVER_CATCH,
	//client
	eGMI_TO_CLIENT_START_ID = 839657,
	eGMI_TO_CLIENT_DICE_TABLE,//to all client and client will know all table,player received this will return a eGMI_TO_SERVER_I_AM_READY.
	eGMI_TO_CLIENT_PLAYER_TURN,
	eGMI_TO_CLIENT_PLAYER_COUNT,
	eGMI_TO_CLIENT_CALL,
	eGMI_TO_CLIENT_CATCH,
	eGMI_TO_CLIENT_NETWORK_ERROR,
	eGMI_MAX,
};


//for eGMI_TO_SERVER_CALL
//struct	sDiceData
//{
//	int iDiceIndex;
//	int	iDiceCount;
//};
//

struct	sGamePacket
{
	char	Data[2048];//maximum for 2k a packet
	int		GetMessageID(){ return *(int*)&Data[0]; }
	void	SetMessageID(int e_iIndex){ memcpy((void*)&Data[0],&e_iIndex,sizeof(int)); }
	//eGMI_TO_CLIENT_START_ID
	int		GetPlayerIndex(){ return *(int*)&Data[4]; }
	void	SetPlayerIndex(int e_iIndex){ memcpy(&Data[4],&e_iIndex,sizeof(int)); }
	int		GetTotalPlayer(){ return *(int*)&Data[8]; }
	void	SetTotalPlayer(int e_iIndex){ memcpy(&Data[8],&e_iIndex,sizeof(int)); }
	//
	int		GetNumPlayerConnected(){  return *(int*)&Data[4];  }
	void	SetNumPlayerConnected(int e_iNum){ memcpy(&Data[4],&e_iNum,sizeof(int)); }
	void	SetDiceTable(char*e_cDiceTable,int e_iCount){memcpy(&Data[8],e_cDiceTable,sizeof(char)*e_iCount);  }
	char*	GetDiceTable(){ return &Data[8]; }
	//eGMI_TO_CLIENT_PLAYER_TURN
	int		GetPlayerTurn(){  return *(int*)&Data[4];  }
	//
	void	SetDiceCount(int e_iCount){  memcpy((void*)&Data[8],&e_iCount,sizeof(int)); }
	int		GetDiceCount(){  return *(int*)&Data[8];  }
	void	SetDicePoint(int e_iPoint){  memcpy((void*)&Data[12],&e_iPoint,sizeof(int)); }
	int		GetDicePoint(){  return *(int*)&Data[12];  }
	//eGMI_TO_CLIENT_CATCH
	void	SetWhichPlayerIsBluffing(int e_iIndex){  memcpy((void*)&Data[8],&e_iIndex,sizeof(int)); }
	int		GetWhichPlayerIsBluffing(){  return *(int*)&Data[8];  }
};

//struct	sAddress
//{
//	int	iAdressp[4];
//};

class	cMultiPlayerNetwork:public cNetwork
{
	friend void					ConnectedThread(size_t _workParameter, size_t _pUri);
	friend void					ConnectedThreadDone(size_t _workParameter, size_t _pUri);
	//if trying to connect true,or false
	bool						m_bDoConnection;
	bool						m_bServer;
	std::string					m_strServerIP;
	//std::vector<sClientAddress>	m_Allo
	void						MessageProcess(sReceivedPacket*e_pPacket);
	std::map<int,_TCPsocket*>	m_PlayerList;
public:
	cMultiPlayerNetwork();
	~cMultiPlayerNetwork();
	int							GetEmptyPlayerIndex();
	int							GetNumPlayerConnected();
	//please call this at right time
	void						Connect();
	//if failed reconnect
	virtual void				Init();
	void						Update(float e_fElpaseTime);
	void						Render();
	using						cNetwork::SendData;
	bool						SendDataToServer(sGamePacket*e_pGamePacket,int e_iSize);
	bool						SendDataToClient(_TCPsocket*e_TCPsocket,sGamePacket*e_pGamePacket);
	using						cNetwork::SendDataToAllClient;
	bool						SendDataToAllClient(sGamePacket*e_pGamePacket,int e_iSize);
	void						ConnectionCreated(bool e_bServer,const char*e_strServerIP);
	void						SetPlayerIDIndex(int e_iIndex,_TCPsocket*e_p_TCPsocket);
	int							GetPlayerIndex(_TCPsocket*e_p_TCPsocket);
	std::vector<int>			m_DiceTable;
};


#endif