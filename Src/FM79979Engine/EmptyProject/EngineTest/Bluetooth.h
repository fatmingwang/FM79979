#pragma once
//https://code.msdn.microsoft.com/windowsdesktop/Bluetooth-Connection-e3263296
#include <mutex>

#include "../../Core/Network/SDL_net.h"
#ifdef WIN32
DEFINE_GUID(GUID_DEVCLASS_BLUETOOTH, { 0xe0cbf06c, 0xcd8b, 0x4647,{ 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } });
#endif
struct sBluetoothPacket
{
	int			iSize;
	char*		pData;
	sBluetoothPacket(int e_iSize,char*e_pData);
	~sBluetoothPacket();
};

class cBluetoothSinglton:public cSingltonTemplate<cBluetoothSinglton>,public cCPP11Thread,public NamedTypedObject
{
	//for server
	bool							CreateSocksetToListenData();
	std::vector<sBluetoothPacket*>	m_BluetoothPacketVector;
	std::mutex						m_BluetoothPacketVectorMutex;
	std::mutex						m_BluetoothSocketVectorMutex;
	//
	_SDLNet_SocketSet*				m_pAllSocketToListenClientMessage;
	std::vector<SDLNet_Socket*>		m_ConnectedSocketVector;
	//
	bool							m_bLostConnection;
	SDLNet_Socket					m_LocalSocket;
	bool							SetDeviceName(const wchar_t*e_strName);
	void							ServerUpdate(float e_fElpaseTime);
	void							ClientUpdate(float e_fElpaseTime);
	void							ConnectToServerUpdate(float e_fElpaseTime);
	//
	cBluetoothSinglton();
	~cBluetoothSinglton();
public:
	SINGLETON_BASIC_FUNCTION(cBluetoothSinglton);
	DEFINE_TYPE_INFO();
	void	Init();
	void	Update(float e_fElpaseTime);
	//
	bool	CreateAsServer(const wchar_t*e_strServerName);
	bool	CreateAsClient(const wchar_t*e_strServerName);
	void	Disconnect();
	void	GetReceivedData(std::vector<sBluetoothPacket*>*e_pDataVector);
	void	FindDevices();
	void	SendDataToAllClient(int e_iLength,char*e_pData);
	void	SendDataToServer(int e_iLength, char*e_pData);
	void	DebugRender();
	void	Android_AddBluetoothPacket(int e_iSize,char*e_pData);
};